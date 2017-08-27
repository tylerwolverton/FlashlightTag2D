#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera, SDL_Renderer* renderer)
{
	StrongActorComponentPtr cameraTransformComponent = currentCamera->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawCameraTransformComponent = std::dynamic_pointer_cast<TransformComponent>(cameraTransformComponent);
	auto cameraPos = rawCameraTransformComponent->GetPosition();

	for (auto actor : gameActors)
	{
		StrongActorComponentPtr graphicsComponent = actor->GetComponentByName(EComponentNames::GraphicsComponentEnum);

		if (graphicsComponent == nullptr)
		{
			continue;
		}

		StrongActorComponentPtr actorTransformComponent = actor->GetComponentByName(EComponentNames::TransformComponentEnum);

		if (actorTransformComponent == nullptr)
		{
			continue;
		}

		std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actorTransformComponent);
		auto actorPos = rawActorTransformComponent->GetPosition();
		auto actorSize = rawActorTransformComponent->GetSize();

		std::shared_ptr<GraphicsComponent> rawGraphicsComponent = std::dynamic_pointer_cast<GraphicsComponent>(graphicsComponent);

		SDL_Rect imgPartRect;
		imgPartRect.x = actorPos.x - cameraPos.x - rawGraphicsComponent->GetImageOffset().x;
		imgPartRect.y = actorPos.y - cameraPos.y - rawGraphicsComponent->GetImageOffset().y;
		imgPartRect.w = actorSize.x;
		imgPartRect.h = actorSize.y;

		SDL_RenderCopy(renderer, rawGraphicsComponent->GetSprite(), &(rawGraphicsComponent->GetAnimationFrameRect()), &imgPartRect);
	}
}

void GraphicsManager::RenderBackground(SDL_Texture* sprite, StrongGameActorPtr currentCamera, SDL_Renderer* renderer, int screenWidth, int screenHeight)
{
	StrongActorComponentPtr cameraTransformComponent = currentCamera->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawCameraTransformComponent = std::dynamic_pointer_cast<TransformComponent>(cameraTransformComponent);
	auto cameraPos = rawCameraTransformComponent->GetPosition();

	int spriteWidth, spriteHeight;
	SDL_QueryTexture(sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	for (int i = 0; i < screenWidth; i += spriteWidth)
	{
		for (int j = 0; j < screenHeight; j += spriteHeight)
		{
			SDL_Rect imgPartRect;
			imgPartRect.x = i - cameraPos.x;
			imgPartRect.y = j - cameraPos.y;
			imgPartRect.w = spriteWidth;
			imgPartRect.h = spriteHeight;

			SDL_RenderCopy(renderer, sprite, NULL, &imgPartRect);
		}
	}
}