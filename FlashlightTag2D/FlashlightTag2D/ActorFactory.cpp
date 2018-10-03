#include "ActorFactory.h"
#include "GameTypes.h"
#include "Vector2D.h"
#include "Actor.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "CharacterLogicComponent.h"
#include "MainMenuLogicComponent.h"
#include "PortalLogicComponent.h"
#include "MouseLogicComponent.h"
#include "InputComponent.h"
#include "CharacterInputComponent.h"
#include "CursorInputComponent.h"
#include "MainMenuInputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "DefaultPhysicsComponent.h"
#include "CharacterPhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "EnemyTouchDamagePhysicsComponent.h"
#include "ProjectilePhysicsComponent.h"
#include "AIComponent.h"
#include "SeekBehavior.h"
#include "RushBehavior.h"
#include "SpawnBehavior.h"
#include "CameraFollowComponent.h"
#include "GameStateComponent.h"
#include "LifeComponent.h"
#include "PlayerLifeComponent.h"
#include "World.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "LevelFactory.h"
#include "Level.h"
#include "ServiceLocator.h"

#include <random>
#include <algorithm>
#include <functional>

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
    m_lastActorId = 0;

    assert(actorList.IsArray());
    for (rapidjson::SizeType i = 0; i < actorList.Size(); i++)
    {
        std::shared_ptr<GameActor> actor;
        if (actorList[i].HasMember("position"))
        {
            actor = createActor(actorList[i]["actor_path"].GetString(), Vector2D<float>(actorList[i]["position"]["x"].GetFloat(),
                                                                                        actorList[i]["position"]["y"].GetFloat()));
        }
        else
        {
            actor = createActor(actorList[i]["actor_path"].GetString());
        }

		m_pEntityMap.insert(std::make_pair(actor->GetActorId(), actor));
		addComponentsToManagers(actor);
    }
    
    m_graphicsMgr->AddCamera(CreateCamera(newLevel->GetLevelSize()));
}

std::shared_ptr<GameActor> ActorFactory::GetActor(ActorId actorId) 
{ 
    auto actorIter = m_pEntityMap.find(actorId);
    if (actorIter != m_pEntityMap.end())
    {
        return actorIter->second;
    }

    return nullptr; 
}

std::shared_ptr<GameActor> ActorFactory::GetPlayer()
{
    for (auto actorIter : m_pEntityMap)
    {
        auto gameStateComp = actorIter.second->GetGameStateComponent();
        if (gameStateComp == nullptr)
        {
            continue;
        }

        if (gameStateComp->GetName() == "Player")
        {
            return actorIter.second;
        }
    }

    return nullptr;
}

void ActorFactory::addComponentsToManagers(std::shared_ptr<GameActor> actor)
{
	auto physicsCompPtr = actor->GetPhysicsComponent();
	if (physicsCompPtr != nullptr)
	{
		m_physicsMgr->AddPhysicsComponentPtr(physicsCompPtr->GetComponentId(), physicsCompPtr);
	}
	auto graphicsCompPtr = actor->GetGraphicsComponent();
	if (graphicsCompPtr != nullptr)
	{
		m_graphicsMgr->AddGraphicsComponentPtr(graphicsCompPtr->GetComponentId(), graphicsCompPtr);
	}
}

std::shared_ptr<GameActor> ActorFactory::createActor(const char* const actorPath, Vector2D<float> position)
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
		addInputComponent(actor["input_component"]["type"].GetString(), newActor);
	}

    if (actor.HasMember("ai_component"))
    {
		std::shared_ptr<Behavior> behavior;
		if (actor["ai_component"].HasMember("behavior"))
		{
			if (!strcmp(actor["ai_component"]["behavior"]["type"].GetString(), "spawn"))
			{
				//std::function<std::shared_ptr<GameActor>(const ActorFactory&, Vector2D<float>)> spawnActor = &ActorFactory::CreateEnemy;
				behavior = std::make_shared<SpawnBehavior>(actor["ai_component"]["behavior"]["delay_time"].GetInt(), actor["ai_component"]["behavior"]["target"].GetString());
			}
			else if (!strcmp(actor["ai_component"]["behavior"]["type"].GetString(), "seek"))
			{

			}
			else if (!strcmp(actor["ai_component"]["behavior"]["type"].GetString(), "rush"))
			{
				behavior = std::make_shared<RushBehavior>();
			}
		}

		newActor->InsertComponent(EComponentNames::AIComponentEnum, std::make_shared<AIComponent>(getNextComponentId(), behavior));
    }

    if (actor.HasMember("transform_component"))
    {
        //rapidjson::Value actorTransform = actorList[i]["transform_component"];
        Vector2D<float> actorPos;
        if (position == Vector2D<float>(-1.0f, -1.0f))
        {
            actorPos = Vector2D<float>(actor["transform_component"]["position"]["x"].GetFloat(),
                                       actor["transform_component"]["position"]["y"].GetFloat());
        }
        else
        {
            actorPos = position;
        }

        auto transformCompPtr = std::make_shared<TransformComponent>(getNextComponentId(),
                                                                     actorPos,
                                                                     Vector2D<float>(actor["transform_component"]["size"]["x"].GetFloat(),
                                                                                     actor["transform_component"]["size"]["y"].GetFloat()),
                                                                     Vector2D<float>(1, 0));

		newActor->InsertComponent(EComponentNames::TransformComponentEnum, transformCompPtr);

        if (actor.HasMember("physics_component"))
        {
			auto physicsCompPtr = addPhysicsComponent(actor["physics_component"]["type"].GetString(), 
													  newActor,
													  transformCompPtr,
													  actor["physics_component"]["max_speed"].GetFloat(),
													  actor["physics_component"]["mass"].GetFloat(),
													  actor["physics_component"]["restitution"].GetFloat());

            if (actor.HasMember("character_logic_component"))
            {
				newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<CharacterLogicComponent>(getNextComponentId(), physicsCompPtr));
            }
        }
        if (actor.HasMember("graphics_component"))
        {
            ComponentId graphicsCompId = getNextComponentId();
            std::shared_ptr<GraphicsComponent> graphicsCompPtr = std::make_shared<GraphicsComponent>(graphicsCompId,
                                                                                                     actor["graphics_component"]["sprite"].GetString(),
                                                                                                     actor["graphics_component"]["animation_speed"].GetInt(),
                                                                                                     transformCompPtr);

            //m_graphicsMgr->AddGraphicsComponentPtr(graphicsCompId, graphicsCompPtr);

			newActor->InsertComponent(EComponentNames::GraphicsComponentEnum, graphicsCompPtr);

            // TODO: Cache changes
            /*m_entityVec.back().m_componentIndexVec[EComponentTypes::Graphics] =
            graphicsMgr.AddGraphicsComponent(actorList[i]["graphics_component"]["sprite"].GetString(),
            actorList[i]["graphics_component"]["animation_speed"].GetInt(),
            transformCompPtr);*/
        }

        if (actor.HasMember("mouse_logic_component"))
        {
			newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<MouseLogicComponent>(getNextComponentId(), transformCompPtr));
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

		newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<MainMenuLogicComponent>(getNextComponentId(), buttonGraphicsCompsMapPtr));
    }
    if (actor.HasMember("portal_logic_component"))
    {
        std::string level = actor["portal_logic_component"]["destination"].GetString();
        newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<PortalLogicComponent>(getNextComponentId(), level));

    }
    if (actor.HasMember("game_state_component"))
    {
        std::shared_ptr<GameStateComponent> gameStateCompPtr = std::make_shared<GameStateComponent>(getNextComponentId(), actorName);

		newActor->InsertComponent(EComponentNames::GameStateComponentEnum, gameStateCompPtr);

        // TODO: Cache changes
        //m_gameStateComponentVec.emplace_back(getNextComponentId(), actorName, role);
        m_gameStateComponentVec.push_back(gameStateCompPtr);
    }
	if (actor.HasMember("life_component"))
	{
		std::shared_ptr<LifeComponent> lifeCompPtr = std::make_shared<LifeComponent>(getNextComponentId(), 
																					 actorId, 
																					 actor["life_component"]["health"].GetInt());

		newActor->InsertComponent(EComponentNames::LifeComponentEnum, lifeCompPtr);
	}
    if (actor.HasMember("player_life_component"))
    {
        std::shared_ptr<PlayerLifeComponent> lifeCompPtr = std::make_shared<PlayerLifeComponent>(getNextComponentId(),
                                                                                                 actorId,
                                                                                                 actor["player_life_component"]["health"].GetInt());

        newActor->InsertComponent(EComponentNames::LifeComponentEnum, lifeCompPtr);
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

void ActorFactory::addInputComponent(const char* const type, std::shared_ptr<GameActor> actor)
{
	if (!strcmp(type, "character_input"))
	{
		actor->InsertComponent(EComponentNames::InputComponentEnum, std::make_shared<CharacterInputComponent>(getNextComponentId()));
	}
	else if (!strcmp(type, "main_menu_input"))
	{
		actor->InsertComponent(EComponentNames::InputComponentEnum, std::make_shared<MainMenuInputComponent>(getNextComponentId()));
	}
	else if (!strcmp(type, "cursor_input"))
	{
		actor->InsertComponent(EComponentNames::InputComponentEnum, std::make_shared<CursorInputComponent>(getNextComponentId()));
	}
}

std::shared_ptr<PhysicsComponent> ActorFactory::addPhysicsComponent(const char* const type,
																	std::shared_ptr<GameActor> actor, 
																	std::shared_ptr<TransformComponent> transformCompPtr,
																	float maxSpeed,
																	float mass,
																	float restitution)
{
    std::shared_ptr<PhysicsComponent> physicsCompPtr;

    if (!strcmp(type, "default_physics_component"))
    {
        physicsCompPtr = std::make_shared<DefaultPhysicsComponent>(getNextComponentId(),
                                                                   transformCompPtr,
                                                                   maxSpeed,
                                                                   mass,
                                                                   restitution);
    }
    else if (!strcmp(type, "character_physics_component"))
    {
        physicsCompPtr = std::make_shared<CharacterPhysicsComponent>(getNextComponentId(),
                                                                     transformCompPtr,
                                                                     maxSpeed,
                                                                     mass,
                                                                     restitution);

    }
    else if (!strcmp(type, "player_physics_component"))
    {
        physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(getNextComponentId(),
                                                                  transformCompPtr,
                                                                  maxSpeed,
                                                                  mass,
                                                                  restitution);

        // TODO: Cache changes
        /*m_entityVec.back().m_componentIndexVec[EComponentTypes::Physics] =
        physicsMgr.AddPlayerPhysicsComponent(transformCompPtr,
        actorList[i]["physics_component"]["max_speed"].GetFloat(),
        actorList[i]["physics_component"]["mass"].GetFloat(),
        actorList[i]["physics_component"]["restitution"].GetFloat());*/

        //physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(physicsComp);
    }
    else if (!strcmp(type, "enemy_touch_damage_physics_component"))
    {
        physicsCompPtr = std::make_shared<EnemyTouchDamagePhysicsComponent>(getNextComponentId(),
                                                                            transformCompPtr,
                                                                            maxSpeed,
                                                                            mass,
                                                                            restitution);
    }
    else if (!strcmp(type, "projectile_physics_component"))
    {
        physicsCompPtr = std::make_shared<ProjectilePhysicsComponent>(getNextComponentId(),
                                                                      transformCompPtr,
                                                                      maxSpeed,
                                                                      mass,
                                                                      restitution);
    }

	actor->InsertComponent(EComponentNames::PhysicsComponentEnum, physicsCompPtr);
	return physicsCompPtr;
}

void ActorFactory::RemoveDeadActors()
{
    bool playerIsDead = false;
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

        std::shared_ptr<GameStateComponent> actorGameStateComp = actorIter->second->GetGameStateComponent();
        if (actorGameStateComp != nullptr
            && actorGameStateComp->GetName() == "Player")
        {
            playerIsDead = true;
        }

		m_pEntityMap.erase(id);
	}

	m_deadActorVec.clear();

    if (playerIsDead)
    {
        auto levelFactory = ServiceLocator::GetLevelFactory();
        if (levelFactory != nullptr)
        {
            levelFactory->ChangeLevel(LevelFactory::LevelPaths::MainMenu);
        }
    }
}

//void ActorFactory::ChooseSeekers(int seekerCount)
//{
//    std::random_device rd;
//    std::mt19937 g(rd());
//
//    std::shuffle(m_gameStateComponentVec.begin(), m_gameStateComponentVec.end(), g);
//
//    for (int i = 0; i < seekerCount && i < m_gameStateComponentVec.size(); i++)
//    {
//        m_gameStateComponentVec[i]->SetRole(EGameBehavior::Seek);
//    }
//}

std::shared_ptr<GameActor> ActorFactory::CreateActorFromName(std::string name, Vector2D<float> position, Vector2D<float> velocity)
{
	std::string actorPath = "resources/actors/" + name + ".json";
    std::shared_ptr<GameActor> actor = createActor(actorPath.c_str());
	actor->GetTransformComponent()->SetPosition(position);
	actor->GetTransformComponent()->SetDirection(velocity.Normalize());
	actor->GetPhysicsComponent()->SetVelocity(velocity);

    m_pEntityMap.insert(std::make_pair(actor->GetActorId(), actor));

    addComponentsToManagers(actor);

    return actor;
}

std::shared_ptr<GameActor> ActorFactory::CreateProjectile(Vector2D<float> position, Vector2D<float> velocity)
{
	std::shared_ptr<GameActor> actor = createActor("resources/actors/projectile.json");
	actor->GetTransformComponent()->SetPosition(position);
	actor->GetTransformComponent()->SetDirection(velocity.Normalize());
	actor->GetPhysicsComponent()->SetVelocity(velocity);

	m_pEntityMap.insert(std::make_pair(actor->GetActorId(), actor));

	addComponentsToManagers(actor);

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
	m_pCurrentCamera = newActor;

    return newActor;
}


