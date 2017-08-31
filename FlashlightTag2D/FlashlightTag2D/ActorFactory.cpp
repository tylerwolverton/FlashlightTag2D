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

	auto physicsCompPtr = std::make_shared<PlayerPhysicsComponent>(transformCompPtr, Vector2D<float>(0, 0), 10, 1);
	components.push_back(std::make_shared<BaseLogicComponent>(physicsCompPtr));
	components.push_back(physicsCompPtr);

	/*
	auto sprite = LoadTexture(world->GetRenderer(), "resources/SpriteSheet.png");

	if (sprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return NULL;
	}*/

	components.push_back(std::make_shared<GraphicsComponent>(1000, transformCompPtr));

	return std::make_shared<GameActor>(components);
}

StrongGameActorPtr ActorFactory::CreateEnemy()
{
	ComponentList components = ComponentList();

	auto transformCompPtr = std::make_shared<TransformComponent>(Vector2D<float>(200.0f, 100.0f), 100.0f);
	components.push_back(transformCompPtr);

	components.push_back(std::make_shared<AIComponent>());

	auto physicsCompPtr = std::make_shared<PhysicsComponent>(transformCompPtr, Vector2D<float>(0,0), 10, 1);
	components.push_back(std::make_shared<BaseLogicComponent>(physicsCompPtr));
	components.push_back(physicsCompPtr);
	
	/*auto sprite = LoadTexture(world->GetRenderer(), "resources/SpriteSheet.png");

	if (sprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return NULL;
	}*/

	components.push_back(std::make_shared<GraphicsComponent>(600, transformCompPtr));

	return std::make_shared<GameActor>(components);
}

StrongGameActorPtr ActorFactory::CreateCamera(StrongGameActorPtr target)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<TransformComponent>(Vector2D<float>(0, 0), Vector2D<float>(World::SCREEN_WIDTH, World::SCREEN_HEIGHT)));
	components.push_back(std::make_shared<FollowTargetAIComponent>(target));

	return std::make_shared<GameActor>(components);
}