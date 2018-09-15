#include "ActorFactory.h"
#include "GameTypes.h"
#include "Vector2D.h"
#include "Actor.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "CharacterLogicComponent.h"
#include "MainMenuLogicComponent.h"
#include "MouseLogicComponent.h"
#include "InputComponent.h"
#include "CharacterInputComponent.h"
#include "MainMenuInputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "AIComponent.h"
#include "CameraFollowComponent.h"
#include "GameStateComponent.h"
#include "World.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "Level.h"
#include "ServiceLocator.h"

#include <random>
#include <algorithm>

// from rapidjson
#include "filereadstream.h"

ActorFactory::ActorFactory(std::shared_ptr<PhysicsManager> physicsMgr, std::shared_ptr<GraphicsManager> graphicsMgr)
    : m_lastActorId(0),
      m_physicsMgr(physicsMgr),
      m_graphicsMgr(graphicsMgr)
{
}

void ActorFactory::InitLevelActors(const rapidjson::Value& actorList, std::shared_ptr<Level> newLevel)
{

    m_physicsMgr->ClearPhysicsComponents();
    m_physicsMgr->SetLevelSize(newLevel->GetLevelSize());

    m_graphicsMgr->Reset();
    m_graphicsMgr->LoadNewLevel(newLevel);

    m_pEntityMap.clear();

    assert(actorList.IsArray());
    for (rapidjson::SizeType i = 0; i < actorList.Size(); i++)
    {
        std::shared_ptr<GameActor> actor = createActor(actorList[i].GetString());
		m_pEntityMap.insert(std::make_pair(actor->GetActorId(), actor));
    }
    
    m_graphicsMgr->AddCamera(CreateCamera(newLevel->GetLevelSize()));
}

std::shared_ptr<GameActor> ActorFactory::createActor(const char* const actorPath)
{
    FILE* fp;
    fopen_s(&fp, actorPath, "rb");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document actor;
    actor.ParseStream(is);
    fclose(fp);

    const char* actorName = actor["name"].GetString();
    ActorId actorId = getNextActorId();
    // TODO: Cache changes
    //m_entityVec.emplace_back(actorId);
    auto newActor = std::make_shared<GameActor>(actorId, actorName);

    auto components = std::vector<std::shared_ptr<ActorComponent>>();
    if (actor.HasMember("input_component"))
    {
        newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
            (EComponentNames::InputComponentEnum, std::make_shared<CharacterInputComponent>(getNextComponentId())));
    }
    if (actor.HasMember("main_menu_input_component"))
    {
        newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
            (EComponentNames::InputComponentEnum, std::make_shared<MainMenuInputComponent>(getNextComponentId())));
    }
    if (actor.HasMember("ai_component"))
    {
        newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
            (EComponentNames::AIComponentEnum, std::make_shared<AIComponent>(getNextComponentId())));
    }
    if (actor.HasMember("transform_component"))
    {
        //rapidjson::Value actorTransform = actorList[i]["transform_component"];
        auto transformCompPtr = std::make_shared<TransformComponent>(getNextComponentId(),
                                                                     Vector2D<float>(actor["transform_component"]["position"]["x"].GetFloat(),
                                                                                     actor["transform_component"]["position"]["y"].GetFloat()),
                                                                     Vector2D<float>(actor["transform_component"]["size"]["x"].GetFloat(),
                                                                                     actor["transform_component"]["size"]["y"].GetFloat()),
                                                                     Vector2D<float>(1, 0));

        newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
            (EComponentNames::TransformComponentEnum, transformCompPtr));

        if (actor.HasMember("physics_component"))
        {
            ComponentId physicsCompId = getNextComponentId();
            std::shared_ptr<PhysicsComponent> physicsCompPtr;

            if (actor["physics_component"]["type"] == "player_physics_component")
            {
                physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(physicsCompId,
                                                                          transformCompPtr,
                                                                          actor["physics_component"]["max_speed"].GetFloat(),
                                                                          actor["physics_component"]["mass"].GetFloat(),
                                                                          actor["physics_component"]["restitution"].GetFloat());

                // TODO: Cache changes
                /*m_entityVec.back().m_componentIndexVec[EComponentTypes::Physics] =
                physicsMgr.AddPlayerPhysicsComponent(transformCompPtr,
                actorList[i]["physics_component"]["max_speed"].GetFloat(),
                actorList[i]["physics_component"]["mass"].GetFloat(),
                actorList[i]["physics_component"]["restitution"].GetFloat());*/

                //physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(physicsComp);
            }
            if (actor["physics_component"]["type"] == "projectile_physics_component")
            {
                physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(physicsCompId,
                                                                          transformCompPtr,
                                                                          actor["physics_component"]["max_speed"].GetFloat(),
                                                                          actor["physics_component"]["mass"].GetFloat(),
                                                                          actor["physics_component"]["restitution"].GetFloat());
            }

            m_physicsMgr->AddPhysicsComponentPtr(physicsCompId, physicsCompPtr);

            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::PhysicsComponentEnum, physicsCompPtr));

            if (actor.HasMember("character_logic_component"))
            {
                newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                    (EComponentNames::LogicComponentEnum, std::make_shared<CharacterLogicComponent>(getNextComponentId(), physicsCompPtr)));
            }
        }

        if (actor.HasMember("graphics_component"))
        {
            ComponentId graphicsCompId = getNextComponentId();
            std::shared_ptr<GraphicsComponent> graphicsCompPtr = std::make_shared<GraphicsComponent>(graphicsCompId,
                                                                                                     actor["graphics_component"]["sprite"].GetString(),
                                                                                                     actor["graphics_component"]["animation_speed"].GetInt(),
                                                                                                     transformCompPtr);

            m_graphicsMgr->AddGraphicsComponentPtr(graphicsCompId, graphicsCompPtr);

            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::GraphicsComponentEnum, graphicsCompPtr));

            // TODO: Cache changes
            /*m_entityVec.back().m_componentIndexVec[EComponentTypes::Graphics] =
            graphicsMgr.AddGraphicsComponent(actorList[i]["graphics_component"]["sprite"].GetString(),
            actorList[i]["graphics_component"]["animation_speed"].GetInt(),
            transformCompPtr);*/
        }

        if (actor.HasMember("mouse_logic_component"))
        {
            newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
                (EComponentNames::LogicComponentEnum, std::make_shared<MouseLogicComponent>(getNextComponentId(), transformCompPtr)));
        }
    }

    if (actor.HasMember("main_menu_logic_component"))
    {
        auto buttonGraphicsCompsMapPtr = std::make_shared<std::unordered_map<std::string, std::shared_ptr<GraphicsComponent>>>();
        const rapidjson::Value& buttons = actor["main_menu_logic_component"]["buttons"];
        for (rapidjson::SizeType i = 0; i < buttons.Size(); i++)
        {
            // TODO: Optimize this
            for (auto actor : m_pEntityMap)
            {
                if (actor.second->GetActorName() != buttons[i]["name"].GetString())
                {
                    continue;
                }

                std::shared_ptr<GraphicsComponent> graphicsComp = actor.second->GetGraphicsComponent();
                if (graphicsComp == nullptr)
                {
                    continue;
                }

                buttonGraphicsCompsMapPtr->emplace(actor.second->GetActorName(), graphicsComp);
                break;
            }
        }

        newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
            (EComponentNames::LogicComponentEnum, std::make_shared<MainMenuLogicComponent>(getNextComponentId(), buttonGraphicsCompsMapPtr)));
    }

    if (actor.HasMember("game_state_component"))
    {
        std::shared_ptr<GameStateComponent> gameStateCompPtr = std::make_shared<GameStateComponent>(getNextComponentId(), actorName, EGameRole::Hider);

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

    //m_pEntityVec.push_back(newActor);

    if (!strcmp(actorName, "Player"))
    {
        m_pCurrentPlayer = newActor;
    }

	return newActor;
}

void ActorFactory::RemoveDeadActors()
{
	for (auto id : m_deadActorVec)
	{
		auto actorIter = m_pEntityMap.find(id);
		if (actorIter == m_pEntityMap.end())
		{
			// Actor already removed
			continue;
		}

		std::shared_ptr<PhysicsComponent> actorPhysComp = actorIter->second->GetPhysicsComponent();
		if (actorPhysComp != nullptr)
		{
			m_physicsMgr->RemovePhysicsComponentPtr(actorPhysComp->GetComponentId());
		}

		std::shared_ptr<GraphicsComponent> actorGraphicsComp = actorIter->second->GetGraphicsComponent();
		if (actorPhysComp != nullptr)
		{
			m_graphicsMgr->RemoveGraphicsComponentPtr(actorGraphicsComp->GetComponentId());
		}

		m_pEntityMap.erase(id);
	}

	m_deadActorVec.clear();
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

std::shared_ptr<GameActor> ActorFactory::CreateProjectile(Vector2D<float> position, Vector2D<float> velocity)
{
	std::shared_ptr<GameActor> actor = createActor("resources/actors/projectile.json");
	actor->GetTransformComponent()->SetPosition(position);
	actor->GetTransformComponent()->SetDirection(velocity.Normalize());
	actor->GetPhysicsComponent()->SetVelocity(velocity);

	m_pEntityMap.insert(std::make_pair(getNextActorId(), actor));

	return actor;
}

std::shared_ptr<GameActor> ActorFactory::CreateCamera(const Vector2D<int>& levelSize)
{
    return CreateCamera(m_pCurrentPlayer, levelSize);
}

std::shared_ptr<GameActor> ActorFactory::CreateCamera(const std::shared_ptr<GameActor>& target, const Vector2D<int>& levelSize)
{
    ActorId actorId = getNextActorId();
    auto newActor = std::make_shared<GameActor>(actorId, "Camera");
    
    auto transformCompPtr = std::make_shared<TransformComponent>(getNextComponentId(),
                                                                 Vector2D<float>(0.0f, 0.0f),
                                                                 Vector2D<float>((float)World::SCREEN_WIDTH, (float)World::SCREEN_HEIGHT),
                                                                 Vector2D<float>(0, 0));
    
    newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
        (EComponentNames::TransformComponentEnum, transformCompPtr));

    auto cameraFollowCompPtr = std::make_shared<CameraFollowComponent>(getNextComponentId(), target, levelSize);
    newActor->m_componentMap.insert(std::make_pair<EComponentNames, std::shared_ptr<ActorComponent>>
        (EComponentNames::CameraFollowComponentEnum, cameraFollowCompPtr));

    //m_pEntityVec.push_back(newActor);
    m_pEntityMap.insert(std::make_pair(actorId, newActor));

    return newActor;
}


