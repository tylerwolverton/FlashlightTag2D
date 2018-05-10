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
#include "CameraFollowComponent.h"
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

void ActorFactory::CreateActorsFromJSONArray(const rapidjson::Value& actorList, PhysicsManager& physicsMgr, GraphicsManager& graphicsMgr, const Vector2D<int>& levelSize)
{
	m_lastActorId = 0;
	//m_entityVec.clear();

    assert(actorList.IsArray());
    for (rapidjson::SizeType i = 0; i < actorList.Size(); i++)
    {
        auto actorName = actorList[i]["name"].GetString();
		ActorId actorId = getNextActorId();
        // TODO: Cache changes
        //m_entityVec.emplace_back(actorId);
        auto newActor = std::make_shared<GameActor>(actorId);

		std::vector<std::shared_ptr<ActorComponent>>  components = std::vector<std::shared_ptr<ActorComponent>>();
		if (actorList[i].HasMember("input_component"))
		{
            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::InputComponentEnum, std::make_shared<InputComponent>(getNextComponentId())));
		}
        if (actorList[i].HasMember("ai_component"))
        {
            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::AIComponentEnum, std::make_shared<AIComponent>(getNextComponentId())));
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
	
            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::TransformComponentEnum, transformCompPtr));

			if (actorList[i].HasMember("physics_component"))
			{
                std::shared_ptr<PhysicsComponent> physicsCompPtr;

                if(actorList[i]["physics_component"]["type"] == "player_physics_component")
                {
                    physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(getNextComponentId(),
                                                              transformCompPtr,
                                                              actorList[i]["physics_component"]["max_speed"].GetFloat(),
                                                              actorList[i]["physics_component"]["mass"].GetFloat(),
                                                              actorList[i]["physics_component"]["restitution"].GetFloat());

                    // TODO: Cache changes
                    /*m_entityVec.back().m_componentIndexVec[EComponentTypes::Physics] =
                            physicsMgr.AddPlayerPhysicsComponent(transformCompPtr,
                                                                 actorList[i]["physics_component"]["max_speed"].GetFloat(),
                                                                 actorList[i]["physics_component"]["mass"].GetFloat(),
                                                                 actorList[i]["physics_component"]["restitution"].GetFloat());*/

				    //physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(physicsComp);
				}

                physicsMgr.AddPhysicsComponentPtr(physicsCompPtr);

                newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                    (EComponentNames::PhysicsComponentEnum, physicsCompPtr));

                if (actorList[i].HasMember("base_logic_component"))
				{
                    newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                        (EComponentNames::BaseLogicComponentEnum, std::make_shared<BaseLogicComponent>(getNextComponentId(), physicsCompPtr)));
				}
			}
			
			if (actorList[i].HasMember("graphics_component"))
			{
                std::shared_ptr<GraphicsComponent> graphicsCompPtr = std::make_shared<GraphicsComponent>(getNextComponentId(),
                                                      actorList[i]["graphics_component"]["sprite"].GetString(),
                                                      actorList[i]["graphics_component"]["animation_speed"].GetInt(),
                                                      transformCompPtr);

                graphicsMgr.AddGraphicsComponentPtr(graphicsCompPtr);

                newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                    (EComponentNames::GraphicsComponentEnum, graphicsCompPtr));

                // TODO: Cache changes
				/*m_entityVec.back().m_componentIndexVec[EComponentTypes::Graphics] = 
									graphicsMgr.AddGraphicsComponent(actorList[i]["graphics_component"]["sprite"].GetString(),
									                                 actorList[i]["graphics_component"]["animation_speed"].GetInt(),
									                                 transformCompPtr);*/
			}
		}
		if (actorList[i].HasMember("game_state_component"))
		{
			std::shared_ptr<GameStateComponent> gameStateCompPtr = std::make_shared<GameStateComponent>(getNextComponentId(), actorName, EGameRole::Hider, levelSize);
            
            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::GameStateComponentEnum, gameStateCompPtr));
            
            // TODO: Cache changes
			//m_gameStateComponentVec.emplace_back(getNextComponentId(), actorName, EGameRole::Hider);
			m_gameStateComponentVec.push_back(gameStateCompPtr);

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

        m_pEntityVec.push_back(newActor);

        if (!strcmp(actorName, "Player"))
        {
            m_pCurrentPlayer = newActor;
        }
    }
}

void ActorFactory::ChooseSeekers(int seekerCount)
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(m_gameStateComponentVec.begin(), m_gameStateComponentVec.end(), g);

	for (int i = 0; i < seekerCount && i < m_gameStateComponentVec.size(); i++)
	{
		m_gameStateComponentVec[i]->SetRole(EGameRole::Seeker);
	}
}

std::shared_ptr<GameActor> ActorFactory::CreateCamera(const Vector2D<int>& levelSize)
{
    return CreateCamera(m_pCurrentPlayer, levelSize);
}

std::shared_ptr<GameActor> ActorFactory::CreateCamera(const std::shared_ptr<GameActor>& target, const Vector2D<int>& levelSize)
{
    auto newActor = std::make_shared<GameActor>(getNextActorId());
    
    auto transformCompPtr = std::make_shared<TransformComponent>(getNextComponentId(),
                                                                 Vector2D<float>(0.0f, 0.0f),
                                                                 Vector2D<float>((float)World::SCREEN_WIDTH, (float)World::SCREEN_HEIGHT),
                                                                 Vector2D<float>(0, 0));
    
    newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>  >
        (EComponentNames::TransformComponentEnum, transformCompPtr));

    auto cameraFollowCompPtr = std::make_shared<CameraFollowComponent>(getNextComponentId(), target, levelSize);
    newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>  >
        (EComponentNames::CameraFollowComponentEnum, cameraFollowCompPtr));

	m_pEntityVec.push_back(newActor);

	return newActor;
}