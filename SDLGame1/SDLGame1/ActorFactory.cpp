#include "ActorFactory.h"
#include "Vector2D.h"
#include "Actor.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "BaseLogicComponent.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "AIComponent.h"
#include "FollowTargetAIComponent.h"
#include "World.h"

using namespace tinyxml2;

ActorFactory::ActorFactory()
{
}

StrongGameActorPtr ActorFactory::CreatePlayer(World* world)
{
	ComponentList components = ComponentList();

	auto transformCompPtr = std::make_shared<TransformComponent>(Vector2D<float>(0, 0), 100.0f);
	components.push_back(transformCompPtr);
	
	components.push_back(std::make_shared<InputComponent>());
	components.push_back(std::make_shared<BaseLogicComponent>(transformCompPtr));

	auto sprite = loadTexture(world->renderer, "resources/SpriteSheet.png");
	if (sprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return NULL;
	}

	components.push_back(std::make_shared<GraphicsComponent>(sprite, 1000, transformCompPtr));

	return std::make_shared<GameActor>(components);
}

StrongGameActorPtr ActorFactory::CreateEnemy(World* world)
{
	ComponentList components = ComponentList();

	auto transformCompPtr = std::make_shared<TransformComponent>(Vector2D<float>(200.0f, 100.0f), 100.0f);
	components.push_back(transformCompPtr);

	components.push_back(std::make_shared<AIComponent>());
	components.push_back(std::make_shared<BaseLogicComponent>(transformCompPtr));
	
	auto sprite = loadTexture(world->renderer, "resources/SpriteSheet.png");
	if (sprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return NULL;
	}

	components.push_back(std::make_shared<GraphicsComponent>(sprite, 6000, transformCompPtr));

	return std::make_shared<GameActor>(components);
}

StrongGameActorPtr ActorFactory::CreateCamera(World* world, StrongGameActorPtr target)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<TransformComponent>(Vector2D<float>(0, 0), Vector2D<float>(World::SCREEN_WIDTH, World::SCREEN_HEIGHT)));
	components.push_back(std::make_shared<FollowTargetAIComponent>(target));

	return std::make_shared<GameActor>(components);
}