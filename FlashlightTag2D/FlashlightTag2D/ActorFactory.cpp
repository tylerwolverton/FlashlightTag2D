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

using namespace tinyxml2;

ActorFactory::ActorFactory()
{
}

StrongGameActorPtr ActorFactory::CreatePlayer()
{
	ComponentList components = ComponentList();

	auto transformCompPtr = std::make_shared<TransformComponent>(Vector2D<float>(0, 0), 100.0f);
	components.push_back(transformCompPtr);

	components.push_back(std::make_shared<InputComponent>());

	auto physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(transformCompPtr, 10.0f, 10.0f);
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

	auto transformCompPtr = std::make_shared<TransformComponent>(position, 100.0f);
	components.push_back(transformCompPtr);

	components.push_back(std::make_shared<AIComponent>());

	auto physicsCompPtr = std::make_shared<PhysicsComponent>(transformCompPtr, 10.0f, 1.0f);
	components.push_back(std::make_shared<BaseLogicComponent>(physicsCompPtr));
	components.push_back(physicsCompPtr);
	
	components.push_back(std::make_shared<GraphicsComponent>("resources/Untitled.png", 300, transformCompPtr));

	components.push_back(std::make_shared<GameStateComponent>("Enemy", role));

	auto newActor = std::make_shared<GameActor>(components);
	m_pEntityList.push_back(newActor);

	return newActor;
}

StrongGameActorPtr ActorFactory::CreateCamera(StrongGameActorPtr target)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<TransformComponent>(Vector2D<float>(0.0f, 0.0f), Vector2D<float>((float)World::SCREEN_WIDTH, (float)World::SCREEN_HEIGHT)));
	components.push_back(std::make_shared<FollowTargetAIComponent>(target));

	auto newActor = std::make_shared<GameActor>(components);
	m_pEntityList.push_back(newActor);

	return newActor;
}