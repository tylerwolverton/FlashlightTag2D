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
#include "GUIItemLogicComponent.h"
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
#include "PickupPhysicsComponent.h"
#include "AIComponent.h"
#include "SeekBehavior.h"
#include "RushBehavior.h"
#include "SpawnBehavior.h"
#include "ShootAtTargetBehavior.h"
#include "Boss1Behavior.h"
#include "CameraFollowComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "LifeComponent.h"
#include "PlayerLifeComponent.h"
#include "Boss1LifeComponent.h"
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
      m_graphicsMgr(graphicsMgr),
      m_persistedPlayerGameStateComp(nullptr),
      m_persistedPlayerLifeComp(nullptr)
{
}

void ActorFactory::InitLevelActors(const rapidjson::Value& actorList, std::shared_ptr<Level> newLevel)
{
    if (m_pCurrentPlayer != nullptr)
    {
        m_persistedPlayerGameStateComp = m_pCurrentPlayer->GetGameStateComponent();
        m_persistedPlayerLifeComp = m_pCurrentPlayer->GetLifeComponent();
    }

    m_physicsMgr->ClearPhysicsComponents();
    m_physicsMgr->LoadNewLevel(newLevel);

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

        if (i == 0)
        {
            m_graphicsMgr->AddCamera(CreateCamera(newLevel->GetLevelSize()));
        }
    }

    if (m_pCurrentPlayer != nullptr)
    {
        if (m_persistedPlayerGameStateComp != nullptr
            && m_persistedPlayerLifeComp != nullptr)
        {
            m_pCurrentPlayer->InsertComponent(EComponentNames::GameStateComponentEnum, m_persistedPlayerGameStateComp);
            m_pCurrentPlayer->InsertComponent(EComponentNames::LifeComponentEnum, m_persistedPlayerLifeComp);
            std::dynamic_pointer_cast<PlayerLifeComponent>(m_persistedPlayerLifeComp)->UpdateHealthBar();
        }

        // TODO: Move this into function
        if (!std::dynamic_pointer_cast<PlayerGameStateComponent>(m_pCurrentPlayer->GetGameStateComponent())->InventoryContainsItem("FirstKey"))
        {
            KillAllActorsByName("FirstKeyInventory");
        }

        if (!std::dynamic_pointer_cast<PlayerGameStateComponent>(m_pCurrentPlayer->GetGameStateComponent())->InventoryContainsItem("SecondKey"))
        {
            KillAllActorsByName("SecondKeyInventory");
        }
    }
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

std::shared_ptr<GameActor> ActorFactory::GetFirstActorWithName(std::string actorName)
{
    for (auto actorIter : m_pEntityMap)
    {
        auto gameStateComp = actorIter.second->GetGameStateComponent();
        if (gameStateComp == nullptr)
        {
            continue;
        }

        if (gameStateComp->GetName() == actorName)
        {
            return actorIter.second;
        }
    }

    return nullptr;
}

std::shared_ptr<GameActor> ActorFactory::GetPlayer()
{
    return GetFirstActorWithName("Player");
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
        std::vector<std::shared_ptr<Behavior>> behaviorVec;
        if (actor["ai_component"].HasMember("behaviors"))
        {
            for (rapidjson::SizeType i = 0; i < actor["ai_component"]["behaviors"].Size(); i++)
            {
                std::string type = actor["ai_component"]["behaviors"][i]["type"].GetString();
                if (!strcmp(type.c_str(), "spawn"))
                {
                    //std::function<std::shared_ptr<GameActor>(const ActorFactory&, Vector2D<float>)> spawnActor = &ActorFactory::CreateEnemy;
                    behaviorVec.push_back(std::make_shared<SpawnBehavior>(actor["ai_component"]["behaviors"][i]["delay_time"].GetInt(), 
                                                                          actor["ai_component"]["behaviors"][i]["target"].GetString()));
                }
                else if (!strcmp(type.c_str(), "seek"))
                {

                }
                else if (!strcmp(type.c_str(), "rush"))
                {
                    behaviorVec.push_back(std::make_shared<RushBehavior>());
                }
                else if (!strcmp(type.c_str(), "shoot_at_target"))
                {
                    // TODO: make this more flexible
                    behaviorVec.push_back(std::make_shared<ShootAtTargetBehavior>(m_pCurrentPlayer, actor["ai_component"]["behaviors"][i]["cooldown"].GetInt()));
                }
                else if (!strcmp(type.c_str(), "boss1"))
                {
                    behaviorVec.push_back(std::make_shared<Boss1Behavior>(m_pCurrentPlayer, Vector2D<int>(actor["ai_component"]["behaviors"][i]["level_size"]["x"].GetInt(), 
                                                                                                          actor["ai_component"]["behaviors"][i]["level_size"]["y"].GetInt())));
                }
            }
        }

        newActor->InsertComponent(EComponentNames::AIComponentEnum, std::make_shared<AIComponent>(getNextComponentId(), behaviorVec));
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
            int animSpeed = -1;
            if (actor["graphics_component"].HasMember("animation_speed"))
            {
                animSpeed = actor["graphics_component"]["animation_speed"].GetInt();
            }
            std::shared_ptr<GraphicsComponent> graphicsCompPtr = std::make_shared<GraphicsComponent>(graphicsCompId,
                                                                                                     actor["graphics_component"]["sprite"].GetString(),
                                                                                                     animSpeed,
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
        auto buttonTransformCompsMapPtr = std::make_shared<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>>();
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

                std::shared_ptr<TransformComponent> transformComp = actor.second->GetTransformComponent();
                if (transformComp == nullptr)
                {
                    continue;
                }

                buttonTransformCompsMapPtr->emplace(actor.second->GetActorName(), transformComp);
                break;
            }
        }

        newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<MainMenuLogicComponent>(getNextComponentId(), buttonTransformCompsMapPtr));
    }
    if (actor.HasMember("gui_item_logic_component"))
    {
        newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<GUIItemLogicComponent>(getNextComponentId()));
    }
    if (actor.HasMember("portal_logic_component"))
    {
        std::string level = actor["portal_logic_component"]["destination"].GetString();
        float countdownTimeSec = -1;
        if (actor["portal_logic_component"].HasMember("countdown_time_sec"))
        {
            countdownTimeSec = actor["portal_logic_component"]["countdown_time_sec"].GetFloat();
        }
        newActor->InsertComponent(EComponentNames::LogicComponentEnum, std::make_shared<PortalLogicComponent>(getNextComponentId(), level, countdownTimeSec));

    }
    if (actor.HasMember("game_state_component"))
    {
        std::string actorType = "";
        if (actor["game_state_component"].HasMember("type"))
        {
            actorType = actor["game_state_component"]["type"].GetString();
        }
        std::shared_ptr<GameStateComponent> gameStateCompPtr = std::make_shared<GameStateComponent>(getNextComponentId(), actorName, actorType);

        newActor->InsertComponent(EComponentNames::GameStateComponentEnum, gameStateCompPtr);

        // TODO: Cache changes
        //m_gameStateComponentVec.emplace_back(getNextComponentId(), actorName, role);
    }
    if (actor.HasMember("player_game_state_component"))
    {
        std::shared_ptr<PlayerGameStateComponent> gameStateCompPtr = std::make_shared<PlayerGameStateComponent>(getNextComponentId(), actorName);

        newActor->InsertComponent(EComponentNames::GameStateComponentEnum, gameStateCompPtr);
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
    if (actor.HasMember("boss1_life_component"))
    {
        std::shared_ptr<Boss1LifeComponent> lifeCompPtr = std::make_shared<Boss1LifeComponent>(getNextComponentId(),
                                                                                               actorId,
                                                                                               actor["boss1_life_component"]["health"].GetInt());

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
    else if (!strcmp(type, "pickup_physics_component"))
    {
        physicsCompPtr = std::make_shared<PickupPhysicsComponent>(getNextComponentId(),
                                                                  transformCompPtr,
                                                                  maxSpeed,
                                                                  mass,
                                                                  restitution);
    }

    actor->InsertComponent(EComponentNames::PhysicsComponentEnum, physicsCompPtr);
    return physicsCompPtr;
}

void ActorFactory::KillAllActorsByName(std::string name)
{
    for (auto actorIter : m_pEntityMap)
    {
        auto gameStateComp = actorIter.second->GetGameStateComponent();
        if (gameStateComp == nullptr)
        {
            continue;
        }

        if (gameStateComp->GetName() == name)
        {
            AddDeadActor(actorIter.second->GetActorId());
            RemoveDeadActors();
            return;
        }
    }
}

void ActorFactory::RemoveDeadActors()
{
    bool playerIsDead = false;
    bool boss1IsDead = false;
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
        if (actorGraphicsComp != nullptr)
        {
            m_graphicsMgr->RemoveGraphicsComponentPtr(actorGraphicsComp->GetComponentId());
        }

        std::shared_ptr<GameStateComponent> actorGameStateComp = actorIter->second->GetGameStateComponent();
        if (actorGameStateComp != nullptr)
        {
            if (actorGameStateComp->GetName() == "Player")
            {
                playerIsDead = true;
            }
            if (actorGameStateComp->GetName() == "Boss1")
            {
                boss1IsDead = true;
            }
        }

        m_pEntityMap.erase(id);
    }

    m_deadActorVec.clear();

    if (playerIsDead)
    {
        auto levelFactory = ServiceLocator::GetLevelFactory();
        if (levelFactory != nullptr)
        {
            m_pCurrentPlayer->GetLifeComponent()->SetHealth(10);
            levelFactory->ChangeLevel(LevelFactory::LevelPaths::LoseScreen);
        }
    }

    if (boss1IsDead)
    {
        auto levelFactory = ServiceLocator::GetLevelFactory();
        if (levelFactory != nullptr)
        {
            m_pCurrentPlayer->GetLifeComponent()->SetHealth(10);
            auto playerGameStateComp = std::dynamic_pointer_cast<PlayerGameStateComponent>(m_pCurrentPlayer->GetGameStateComponent());
            playerGameStateComp->RemoveFromInventoryByName("FirstKey");
            playerGameStateComp->RemoveFromInventoryByName("SecondKey");
            levelFactory->ChangeLevel(LevelFactory::LevelPaths::WinScreen);
        }
    }
}

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


