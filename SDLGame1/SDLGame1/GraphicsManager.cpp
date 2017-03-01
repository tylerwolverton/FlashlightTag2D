#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "GameActor.h"


GraphicsManager::GraphicsManager()
{
}


GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera, SDL_Renderer* renderer)
{
	for (auto actor : gameActors)
	{
		StrongActorComponentPtr graphicsComponent = actor->GetComponentByName(EComponentNames::GraphicsComponentEnum);

		if (graphicsComponent != nullptr)
		{
			SDL_Rect imgPartRect;
			imgPartRect.x = actor->m_position.x - currentCamera->m_position.x;
			imgPartRect.y = actor->m_position.y - currentCamera->m_position.y;
			imgPartRect.w = actor->m_size.x;
			imgPartRect.h = actor->m_size.y;

			std::shared_ptr<GraphicsComponent> rawGraphicsComponent = std::dynamic_pointer_cast<GraphicsComponent>(graphicsComponent);

			SDL_RenderCopy(renderer, actor->m_sprite, &(rawGraphicsComponent->animationFrameRect), &imgPartRect);
		}
	}
}