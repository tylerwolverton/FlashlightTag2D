#include "ActorFactory.h"
#include "GameTypes.h"
#include "Vector2D.h"
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

ActorFactory::ActorFactory(std::shared_ptr<PhysicsManager> physicsMgrPtr, std::shared_ptr<GraphicsManager> graphicsMgrPtr)
    : m_lastActorId(0),
      m_physicsMgrPtr(physicsMgrPtr),
      m_graphicsMgrPtr(graphicsMgrPtr),
      m_persistedPlayerGameStateCompPtr(nullptr),
      m_persistedPlayerLifeCompPtr(nullptr)
{
}

void ActorFactory::InitLevelActors(const rapidjson::Value& actorList, std::shared_ptr<Level> newLevelPtr)
{
    if (m_curPlayerPtr != nullptr)
    {
        m_persistedPlayerGameStateCompPtr = m_curPlayerPtr->GetGameStateCompPtr();
        m_persistedPlayerLifeCompPtr = m_curPlayerPtr->GetLifeCompPtr();
    }

    m_physicsMgrPtr->ClearPhysicsComponents();
    m_physicsMgrPtr->LoadNewLevel(newLevelPtr);

    m_graphicsMgrPtr->Reset();
    m_graphicsMgrPtr->LoadNewLevel(newLevelPtr);

    m_idToActorPtrMap.clear();
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

        m_idToActorPtrMap.insert(std::make_pair(actor->GetActorId(), actor));
        addComponentsToManagers(actor);

        if (i == 0)
        {
            m_graphicsMgrPtr->AddCamera(CreateCamera(newLevelPtr->GetLevelSize()));
        }
    }

    if (m_curPlayerPtr != nullptr)
    {
        if (m_persistedPlayerGameStateCompPtr != nullptr
            && m_persistedPlayerLifeCompPtr != nullptr)
        {
            m_curPlayerPtr->InsertCompPtr(EComponentNames::GameStateComponentEnum, m_persistedPlayerGameStateCompPtr);
            m_curPlayerPtr->InsertCompPtr(EComponentNames::LifeComponentEnum, m_persistedPlayerLifeCompPtr);
            std::dynamic_pointer_cast<PlayerLifeComponent>(m_persistedPlayerLifeCompPtr)->UpdateHealthBar();
        }

        // TODO: Move this into function
        if (!std::dynamic_pointer_cast<PlayerGameStateComponent>(m_curPlayerPtr->GetGameStateCompPtr())->InventoryContainsItem("FirstKey"))
        {
            KillAllActorsByName("FirstKeyInventory");
        }

        if (!std::dynamic_pointer_cast<PlayerGameStateComponent>(m_curPlayerPtr->GetGameStateCompPtr())->InventoryContainsItem("SecondKey"))
        {
            KillAllActorsByName("SecondKeyInventory");
        }
    }
}

std::shared_ptr<GameActor> ActorFactory::GetActor(ActorId actorId) 
{ 
    auto actorMapIter = m_idToActorPtrMap.find(actorId);
    if (actorMapIter != m_idToActorPtrMap.end())
    {
        return actorMapIter->second;
    }

    return nullptr; 
}

std::shared_ptr<GameActor> ActorFactory::GetFirstActorWithName(std::string actorName)
{
    for (auto actorMapIter : m_idToActorPtrMap)
    {
        auto gameStateComp = actorMapIter.second->GetGameStateCompPtr();
        if (gameStateComp == nullptr)
        {
            continue;
        }

        if (gameStateComp->GetName() == actorName)
        {
            return actorMapIter.second;
        }
    }

    return nullptr;
}

std::shared_ptr<GameActor> ActorFactory::GetPlayer()
{
    return GetFirstActorWithName("Player");
}

void ActorFactory::addComponentsToManagers(std::shared_ptr<GameActor> actorPtr)
{
    auto physicsCompPtr = actorPtr->GetPhysicsCompPtr();
    if (physicsCompPtr != nullptr)
    {
        m_physicsMgrPtr->AddPhysicsComponentPtr(physicsCompPtr->GetComponentId(), physicsCompPtr);
    }
    auto graphicsCompPtr = actorPtr->GetGraphicsCompPtr();
    if (graphicsCompPtr != nullptr)
    {
        m_graphicsMgrPtr->AddGraphicsComponentPtr(graphicsCompPtr->GetComponentId(), graphicsCompPtr);
    }
}

std::shared_ptr<GameActor> ActorFactory::createActor(const char* const actorPath, Vector2D<float> position)
{
    FILE* fp;
    fopen_s(&fp, actorPath, "rb");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document actorJson;
    actorJson.ParseStream(is);
    fclose(fp);

    const char* actorName = actorJson["name"].GetString();
    ActorId actorId = getNextActorId();
    // TODO: Cache changes
    //m_entityVec.emplace_back(actorId);
    auto newActorPtr = std::make_shared<GameActor>(actorId, actorName);

    auto components = std::vector<std::shared_ptr<ActorComponent>>();
    if (actorJson.HasMember("input_component"))
    {
        addInputComponent(actorJson["input_component"]["type"].GetString(), newActorPtr);
    }

    if (actorJson.HasMember("ai_component"))
    {
        std::vector<std::shared_ptr<Behavior>> behaviorPtrVec;
        if (actorJson["ai_component"].HasMember("behaviors"))
        {
            for (rapidjson::SizeType i = 0; i < actorJson["ai_component"]["behaviors"].Size(); i++)
            {
                std::string type = actorJson["ai_component"]["behaviors"][i]["type"].GetString();
                if (!strcmp(type.c_str(), "spawn"))
                {
                    behaviorPtrVec.push_back(std::make_shared<SpawnBehavior>(actorJson["ai_component"]["behaviors"][i]["delay_time"].GetInt(), 
                                                                          actorJson["ai_component"]["behaviors"][i]["target"].GetString()));
                }
                else if (!strcmp(type.c_str(), "seek"))
                {

                }
                else if (!strcmp(type.c_str(), "rush"))
                {
                    behaviorPtrVec.push_back(std::make_shared<RushBehavior>());
                }
                else if (!strcmp(type.c_str(), "shoot_at_target"))
                {
                    // TODO: make this more flexible
                    behaviorPtrVec.push_back(std::make_shared<ShootAtTargetBehavior>(m_curPlayerPtr, actorJson["ai_component"]["behaviors"][i]["cooldown"].GetInt()));
                }
                else if (!strcmp(type.c_str(), "boss1"))
                {
                    behaviorPtrVec.push_back(std::make_shared<Boss1Behavior>(m_curPlayerPtr, Vector2D<int>(actorJson["ai_component"]["behaviors"][i]["level_size"]["x"].GetInt(), 
                                                                                                          actorJson["ai_component"]["behaviors"][i]["level_size"]["y"].GetInt())));
                }
            }
        }

        newActorPtr->InsertCompPtr(EComponentNames::AIComponentEnum, std::make_shared<AIComponent>(getNextComponentId(), behaviorPtrVec));
    }

    if (actorJson.HasMember("transform_component"))
    {
        //rapidjson::Value actorTransform = actorList[i]["transform_component"];
        Vector2D<float> actorPos;
        if (position == Vector2D<float>(-1.0f, -1.0f))
        {
            actorPos = Vector2D<float>(actorJson["transform_component"]["position"]["x"].GetFloat(),
                                       actorJson["transform_component"]["position"]["y"].GetFloat());
        }
        else
        {
            actorPos = position;
        }

        auto transformCompPtr = std::make_shared<TransformComponent>(getNextComponentId(),
                                                                     actorPos,
                                                                     Vector2D<float>(actorJson["transform_component"]["size"]["x"].GetFloat(),
                                                                                     actorJson["transform_component"]["size"]["y"].GetFloat()),
                                                                     Vector2D<float>(1, 0));

        newActorPtr->InsertCompPtr(EComponentNames::TransformComponentEnum, transformCompPtr);

        if (actorJson.HasMember("physics_component"))
        {
            auto physicsCompPtr = addPhysicsComponent(actorJson["physics_component"]["type"].GetString(), 
                                                      newActorPtr,
                                                      transformCompPtr,
                                                      actorJson["physics_component"]["max_speed"].GetFloat(),
                                                      actorJson["physics_component"]["mass"].GetFloat(),
                                                      actorJson["physics_component"]["restitution"].GetFloat());

            if (actorJson.HasMember("character_logic_component"))
            {
                newActorPtr->InsertCompPtr(EComponentNames::LogicComponentEnum, std::make_shared<CharacterLogicComponent>(getNextComponentId(), physicsCompPtr));
            }
        }
        if (actorJson.HasMember("graphics_component"))
        {
            ComponentId graphicsCompId = getNextComponentId();
            int animSpeed = -1;
            if (actorJson["graphics_component"].HasMember("animation_speed"))
            {
                animSpeed = actorJson["graphics_component"]["animation_speed"].GetInt();
            }
            std::shared_ptr<GraphicsComponent> graphicsCompPtr = std::make_shared<GraphicsComponent>(graphicsCompId,
                                                                                                     actorJson["graphics_component"]["sprite"].GetString(),
                                                                                                     animSpeed,
                                                                                                     transformCompPtr);

            newActorPtr->InsertCompPtr(EComponentNames::GraphicsComponentEnum, graphicsCompPtr);

            // TODO: Cache changes
            /*m_entityVec.back().m_componentIndexVec[EComponentTypes::Graphics] =
            graphicsMgr.AddGraphicsComponent(actorList[i]["graphics_component"]["sprite"].GetString(),
            actorList[i]["graphics_component"]["animation_speed"].GetInt(),
            transformCompPtr);*/
        }

        if (actorJson.HasMember("mouse_logic_component"))
        {
            newActorPtr->InsertCompPtr(EComponentNames::LogicComponentEnum, std::make_shared<MouseLogicComponent>(getNextComponentId(), transformCompPtr));
        }
    }
    if (actorJson.HasMember("main_menu_logic_component"))
    {
        auto buttonTransformCompsMapPtr = std::make_shared<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>>();
        const rapidjson::Value& buttons = actorJson["main_menu_logic_component"]["buttons"];
        for (rapidjson::SizeType i = 0; i < buttons.Size(); i++)
        {
            // TODO: Optimize this
            for (auto actor : m_idToActorPtrMap)
            {
                if (actor.second->GetActorName() != buttons[i]["name"].GetString())
                {
                    continue;
                }

                std::shared_ptr<TransformComponent> transformComp = actor.second->GetTransformCompPtr();
                if (transformComp == nullptr)
                {
                    continue;
                }

                buttonTransformCompsMapPtr->emplace(actor.second->GetActorName(), transformComp);
                break;
            }
        }

        newActorPtr->InsertCompPtr(EComponentNames::LogicComponentEnum, std::make_shared<MainMenuLogicComponent>(getNextComponentId(), buttonTransformCompsMapPtr));
    }
    if (actorJson.HasMember("gui_item_logic_component"))
    {
        newActorPtr->InsertCompPtr(EComponentNames::LogicComponentEnum, std::make_shared<GUIItemLogicComponent>(getNextComponentId()));
    }
    if (actorJson.HasMember("portal_logic_component"))
    {
        std::string level = actorJson["portal_logic_component"]["destination"].GetString();
        float countdownTimeSec = -1;
        if (actorJson["portal_logic_component"].HasMember("countdown_time_sec"))
        {
            countdownTimeSec = actorJson["portal_logic_component"]["countdown_time_sec"].GetFloat();
        }
        newActorPtr->InsertCompPtr(EComponentNames::LogicComponentEnum, std::make_shared<PortalLogicComponent>(getNextComponentId(), level, countdownTimeSec));

    }
    if (actorJson.HasMember("game_state_component"))
    {
        std::string actorType = "";
        if (actorJson["game_state_component"].HasMember("type"))
        {
            actorType = actorJson["game_state_component"]["type"].GetString();
        }
        std::shared_ptr<GameStateComponent> gameStateCompPtr = std::make_shared<GameStateComponent>(getNextComponentId(), actorName, actorType);

        newActorPtr->InsertCompPtr(EComponentNames::GameStateComponentEnum, gameStateCompPtr);

        // TODO: Cache changes
        //m_gameStateComponentVec.emplace_back(getNextComponentId(), actorName, role);
    }
    if (actorJson.HasMember("player_game_state_component"))
    {
        std::shared_ptr<PlayerGameStateComponent> gameStateCompPtr = std::make_shared<PlayerGameStateComponent>(getNextComponentId(), actorName);

        newActorPtr->InsertCompPtr(EComponentNames::GameStateComponentEnum, gameStateCompPtr);
    }
    if (actorJson.HasMember("life_component"))
    {
        std::shared_ptr<LifeComponent> lifeCompPtr = std::make_shared<LifeComponent>(getNextComponentId(), 
                                                                                     actorId, 
                                                                                     actorJson["life_component"]["health"].GetInt());

        newActorPtr->InsertCompPtr(EComponentNames::LifeComponentEnum, lifeCompPtr);
    }
    if (actorJson.HasMember("player_life_component"))
    {
        std::shared_ptr<PlayerLifeComponent> lifeCompPtr = std::make_shared<PlayerLifeComponent>(getNextComponentId(),
                                                                                                 actorId,
                                                                                                 actorJson["player_life_component"]["health"].GetInt());

        newActorPtr->InsertCompPtr(EComponentNames::LifeComponentEnum, lifeCompPtr);
    }
    if (actorJson.HasMember("boss1_life_component"))
    {
        std::shared_ptr<Boss1LifeComponent> lifeCompPtr = std::make_shared<Boss1LifeComponent>(getNextComponentId(),
                                                                                               actorId,
                                                                                               actorJson["boss1_life_component"]["health"].GetInt());

        newActorPtr->InsertCompPtr(EComponentNames::LifeComponentEnum, lifeCompPtr);
    }
    //if (actorList[i].HasMember("follow_target_ai_component"))
    //{
    //	StrongActorPtr target = nullptr;
    //	auto targetName = actorList[i]["follow_target_ai_component"]["target_name"].GetString();
    //	for (auto entity : m_pEntityList)
    //	{
    //		auto gameStateComp = entity->GetGameStateCompPtr();
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
        m_curPlayerPtr = newActorPtr;
    }

    return newActorPtr;
}

void ActorFactory::addInputComponent(const char* const type, std::shared_ptr<GameActor> actorPtr)
{
    if (!strcmp(type, "character_input"))
    {
        actorPtr->InsertCompPtr(EComponentNames::InputComponentEnum, std::make_shared<CharacterInputComponent>(getNextComponentId()));
    }
    else if (!strcmp(type, "main_menu_input"))
    {
        actorPtr->InsertCompPtr(EComponentNames::InputComponentEnum, std::make_shared<MainMenuInputComponent>(getNextComponentId()));
    }
    else if (!strcmp(type, "cursor_input"))
    {
        actorPtr->InsertCompPtr(EComponentNames::InputComponentEnum, std::make_shared<CursorInputComponent>(getNextComponentId()));
    }
}

std::shared_ptr<PhysicsComponent> ActorFactory::addPhysicsComponent(const char* const type,
                                                                    std::shared_ptr<GameActor> actorPtr, 
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

    actorPtr->InsertCompPtr(EComponentNames::PhysicsComponentEnum, physicsCompPtr);
    return physicsCompPtr;
}

void ActorFactory::KillAllActorsByName(std::string name)
{
    for (auto actorIter : m_idToActorPtrMap)
    {
        auto gameStateCompPtr = actorIter.second->GetGameStateCompPtr();
        if (gameStateCompPtr == nullptr)
        {
            continue;
        }

        if (gameStateCompPtr->GetName() == name)
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
    for (auto id : m_deadActorIdVec)
    {
        auto actorMapIter = m_idToActorPtrMap.find(id);
        if (actorMapIter == m_idToActorPtrMap.end())
        {
            // Actor already removed
            continue;
        }

        auto actorPhysCompPtr = actorMapIter->second->GetPhysicsCompPtr();
        if (actorPhysCompPtr != nullptr)
        {
            m_physicsMgrPtr->RemovePhysicsComponentPtr(actorPhysCompPtr->GetComponentId());
        }

        auto actorGraphicsCompPtr = actorMapIter->second->GetGraphicsCompPtr();
        if (actorGraphicsCompPtr != nullptr)
        {
            m_graphicsMgrPtr->RemoveGraphicsComponentPtr(actorGraphicsCompPtr->GetComponentId());
        }

        auto actorGameStateCompPtr = actorMapIter->second->GetGameStateCompPtr();
        if (actorGameStateCompPtr != nullptr)
        {
            if (actorGameStateCompPtr->GetName() == "Player")
            {
                playerIsDead = true;
            }
            if (actorGameStateCompPtr->GetName() == "Boss1")
            {
                boss1IsDead = true;
            }
        }

        m_idToActorPtrMap.erase(id);
    }

    m_deadActorIdVec.clear();

    if (playerIsDead)
    {
        auto levelFactoryPtr = ServiceLocator::GetLevelFactory();
        if (levelFactoryPtr != nullptr)
        {
            m_curPlayerPtr->GetLifeCompPtr()->SetHealth(10);
            levelFactoryPtr->ChangeLevel(LevelFactory::LevelPaths::LoseScreen);
        }
    }

    if (boss1IsDead)
    {
        auto levelFactoryPtr = ServiceLocator::GetLevelFactory();
        if (levelFactoryPtr != nullptr)
        {
            m_curPlayerPtr->GetLifeCompPtr()->SetHealth(10);
            auto playerGameStateComp = std::dynamic_pointer_cast<PlayerGameStateComponent>(m_curPlayerPtr->GetGameStateCompPtr());
            playerGameStateComp->RemoveFromInventoryByName("FirstKey");
            playerGameStateComp->RemoveFromInventoryByName("SecondKey");
            levelFactoryPtr->ChangeLevel(LevelFactory::LevelPaths::WinScreen);
        }
    }
}

std::shared_ptr<GameActor> ActorFactory::CreateActorFromName(std::string name, Vector2D<float> position, Vector2D<float> velocity)
{
    std::string actorPath = "resources/actors/" + name + ".json";
    std::shared_ptr<GameActor> actor = createActor(actorPath.c_str());
    actor->GetTransformCompPtr()->SetPosition(position);
    actor->GetTransformCompPtr()->SetDirection(velocity.Normalize());
    actor->GetPhysicsCompPtr()->SetVelocity(velocity);

    m_idToActorPtrMap.insert(std::make_pair(actor->GetActorId(), actor));

    addComponentsToManagers(actor);

    return actor;
}

std::shared_ptr<GameActor> ActorFactory::CreateProjectile(Vector2D<float> position, Vector2D<float> velocity)
{
    std::shared_ptr<GameActor> actor = createActor("resources/actors/projectile.json");
    actor->GetTransformCompPtr()->SetPosition(position);
    actor->GetTransformCompPtr()->SetDirection(velocity.Normalize());
    actor->GetPhysicsCompPtr()->SetVelocity(velocity);

    m_idToActorPtrMap.insert(std::make_pair(actor->GetActorId(), actor));

    addComponentsToManagers(actor);

    return actor;
}

std::shared_ptr<GameActor> ActorFactory::CreateCamera(const Vector2D<int>& levelSize)
{
    return CreateCamera(m_curPlayerPtr, levelSize);
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
    m_idToActorPtrMap.insert(std::make_pair(actorId, newActor));
    m_curCameraPtr = newActor;

    return newActor;
}


