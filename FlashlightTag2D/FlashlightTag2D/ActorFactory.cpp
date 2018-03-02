#include "ActorFactory.h"
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

ActorFactory::ActorFactory()
{
}

void ActorFactory::CreateActorsFromJSONArray(const rapidjson::Value& actorList)
{
    assert(actorList.IsArray());
    for (rapidjson::SizeType i = 0; i < actorList.Size(); i++)
    {
        auto actorName = actorList[i]["name"].GetString();

        ComponentList components = ComponentList();

        auto transformCompPtr = std::make_shared<TransformComponent>(Vector2D<float>(actorList[i]["position"]["x"].GetFloat(), 
                                                                                     actorList[i]["position"]["y"].GetFloat()),
                                                                     Vector2D<float>(actorList[i]["size"]["x"].GetFloat(),
                                                                                     actorList[i]["size"]["y"].GetFloat()), 
                                                                     Vector2D<float>(1, 0));
        components.push_back(transformCompPtr);

        if (!strcmp(actorName, "Player"))
        {
            components.push_back(std::make_shared<InputComponent>());
        }

        auto physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(transformCompPtr, 
                                                                       actorList[i]["max_speed"].GetFloat(), 
                                                                       actorList[i]["mass"].GetFloat(),
                                                                       actorList[i]["restitution"].GetFloat());
        components.push_back(std::make_shared<BaseLogicComponent>(physicsCompPtr));
        components.push_back(physicsCompPtr);

        components.push_back(std::make_shared<GraphicsComponent>(actorList[i]["sprite"].GetString(), 
                                                                 actorList[i]["animation_speed"].GetInt(), 
                                                                 transformCompPtr));

        components.push_back(std::make_shared<GameStateComponent>(actorName, EGameRole::Hider));

        auto newActor = std::make_shared<GameActor>(components);
        m_pEntityList.push_back(newActor);

        if (!strcmp(actorName, "Player"))
        {
            m_pCurrentPlayer = newActor;
        }
    }
}

StrongGameActorPtr ActorFactory::CreatePlayer()
{
	ComponentList components = ComponentList();

	auto transformCompPtr = std::make_shared<TransformComponent>(Vector2D<float>(0, 0), 100.0f, Vector2D<float>(1, 0));
	components.push_back(transformCompPtr);

	components.push_back(std::make_shared<InputComponent>());

	auto physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(transformCompPtr, 10.0f, 1.0f, .5f);
	components.push_back(std::make_shared<BaseLogicComponent>(physicsCompPtr));
	components.push_back(physicsCompPtr);
	
	components.push_back(std::make_shared<GraphicsComponent>("resources/background.png", 500, transformCompPtr));

	components.push_back(std::make_shared<GameStateComponent>("Player", EGameRole::Hider));

	auto newActor = std::make_shared<GameActor>(components);
	m_pEntityList.push_back(newActor);

	return newActor;
}

StrongGameActorPtr ActorFactory::CreateEnemy(Vector2D<float> position, EGameRole role)
{
	ComponentList components = ComponentList();

	auto transformCompPtr = std::make_shared<TransformComponent>(position, 100.0f, Vector2D<float>(1, 0));
	components.push_back(transformCompPtr);

	components.push_back(std::make_shared<AIComponent>());

	auto physicsCompPtr = std::make_shared<PhysicsComponent>(transformCompPtr, 10.0f, 1.0f, .5f);
	components.push_back(std::make_shared<BaseLogicComponent>(physicsCompPtr));
	components.push_back(physicsCompPtr);
	
	components.push_back(std::make_shared<GraphicsComponent>("resources/background.png", 300, transformCompPtr));

	components.push_back(std::make_shared<GameStateComponent>("Enemy", role));

	auto newActor = std::make_shared<GameActor>(components);
	m_pEntityList.push_back(newActor);

	return newActor;
}

StrongGameActorPtr ActorFactory::CreateCamera()
{
    return CreateCamera(m_pCurrentPlayer);
}

StrongGameActorPtr ActorFactory::CreateCamera(StrongGameActorPtr target)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<TransformComponent>(Vector2D<float>(0.0f, 0.0f), Vector2D<float>((float)World::SCREEN_WIDTH, (float)World::SCREEN_HEIGHT), Vector2D<float>(0, 0)));
	components.push_back(std::make_shared<FollowTargetAIComponent>(target));

	auto newActor = std::make_shared<GameActor>(components);
	m_pEntityList.push_back(newActor);

	return newActor;
}