#include "ActorFactory.h"
#include "GameTypes.h"
#include "Vector2D.h"
#include "Actor.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "BaseLogicComponent.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "AIComponent.h"
#include "FollowTargetAIComponent.h"
#include "GameStateComponent.h"
#include "World.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"

#include <random>
#include <algorithm>

ActorFactory::ActorFactory()
    : m_lastActorId(0)
{
}

void ActorFactory::CreateActorsFromJSONArray(const rapidjson::Value& actorList, PhysicsManager& physicsMgr, GraphicsManager& graphicsMgr)
{
    assert(actorList.IsArray());
    for (rapidjson::SizeType i = 0; i < actorList.Size(); i++)
    {
        auto actorName = actorList[i]["name"].GetString();

        ComponentList components = ComponentList();
		if (actorList[i].HasMember("input_component"))
		{
			components.push_back(std::make_shared<InputComponent>(getNextComponentId()));
		}
        if (actorList[i].HasMember("ai_component"))
        {
            components.push_back(std::make_shared<AIComponent>(getNextComponentId()));
        }
		if(actorList[i].HasMember("transform_component"))
		{ 
			//rapidjson::Value actorTransform = actorList[i]["transform_component"];
			auto transformCompPtr = std::make_shared<TransformComponent>(getNextComponentId(),
                                                                         Vector2D<float>(actorList[i]["transform_component"]["position"]["x"].GetFloat(),
																						 actorList[i]["transform_component"]["position"]["y"].GetFloat()),
																		 Vector2D<float>(actorList[i]["transform_component"]["size"]["x"].GetFloat(),
																						 actorList[i]["transform_component"]["size"]["y"].GetFloat()),
																		 Vector2D<float>(1, 0));
			components.push_back(transformCompPtr);
					
			if (actorList[i].HasMember("physics_component"))
			{
                auto physicsComp = PlayerPhysicsComponent(getNextComponentId(),
                                                          transformCompPtr,
                                                          actorList[i]["physics_component"]["max_speed"].GetFloat(),
                                                          actorList[i]["physics_component"]["mass"].GetFloat(),
                                                          actorList[i]["physics_component"]["restitution"].GetFloat());


				auto physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(physicsComp);
				
                physicsMgr.AddPhysicsComponent(physicsComp);
                
                if (actorList[i].HasMember("base_logic_component"))
				{
					components.push_back(std::make_shared<BaseLogicComponent>(getNextComponentId(), physicsCompPtr));
				}
				components.push_back(physicsCompPtr);
			}
			
			if (actorList[i].HasMember("graphics_component"))
			{
                auto graphicsComp = GraphicsComponent(getNextComponentId(),
                                                      actorList[i]["graphics_component"]["sprite"].GetString(),
                                                      actorList[i]["graphics_component"]["animation_speed"].GetInt(),
                                                      transformCompPtr);

                graphicsMgr.AddGraphicsComponent(graphicsComp);

				components.push_back(std::make_shared<GraphicsComponent>(graphicsComp));
			}
		}
		if (actorList[i].HasMember("game_state_component"))
		{
			StrongGameStateComponentPtr gameStateComp = std::make_shared<GameStateComponent>(getNextComponentId(), actorName, EGameRole::Hider);

			//m_gameStateComponentVec.emplace_back(getNextComponentId(), actorName, EGameRole::Hider);
			m_gameStateComponentVec.push_back(gameStateComp);

			components.push_back(gameStateComp);
		}

		//if (actorList[i].HasMember("follow_target_ai_component"))
		//{
		//	StrongActorPtr target = nullptr;
		//	auto targetName = actorList[i]["follow_target_ai_component"]["target_name"].GetString();
		//	for (auto entity : m_pEntityList)
		//	{
		//		auto gameStateComp = entity->GetGameStateComponent();
		//		if (gameStateComp == nullptr)
		//		{
		//			continue;
		//		}
		//		if (targetName == gameStateComp->GetName())
		//		{
		//			// Set to first occurance of target
		//			target = entity;
		//			break;
		//		}
		//	}
		//	if (target != nullptr)
		//	{
		//		components.push_back(std::make_shared<FollowTargetAIComponent>(target));
		//	}
		//}

        auto newActor = std::make_shared<GameActor>(getNextActorId(), components);
        m_pEntityList.push_back(newActor);

        if (!strcmp(actorName, "Player"))
        {
            m_pCurrentPlayer = newActor;
        }
    }
}

void ActorFactory::ChooseSeekers(int seekerCount)
{
	//auto shuffledVector = std::vector<GameStateComponent>(m_gameStateComponentVec);

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(m_gameStateComponentVec.begin(), m_gameStateComponentVec.end(), g);

	for (int i = 0; i < seekerCount && i < m_gameStateComponentVec.size(); i++)
	{
		m_gameStateComponentVec[i]->SetRole(EGameRole::Seeker);
	}
}

StrongGameActorPtr ActorFactory::CreateCamera()
{
    return CreateCamera(m_pCurrentPlayer);
}

StrongGameActorPtr ActorFactory::CreateCamera(const StrongGameActorPtr& target)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<TransformComponent>(getNextComponentId(), Vector2D<float>(0.0f, 0.0f), Vector2D<float>((float)World::SCREEN_WIDTH, (float)World::SCREEN_HEIGHT), Vector2D<float>(0, 0)));
	components.push_back(std::make_shared<FollowTargetAIComponent>(getNextComponentId(), target));

	auto newActor = std::make_shared<GameActor>(getNextActorId(), components);
	m_pEntityList.push_back(newActor);

	return newActor;
}