#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

GraphicsComponent::GraphicsComponent(SDL_Texture* sprite, int animationTimer, std::shared_ptr<TransformComponent> transformComponent)
	: m_sprite(sprite),
	  m_animationTimer(animationTimer),
	  m_TransformComponent(transformComponent),
	  imageOffset(transformComponent->GetSize() / 2)
{
	xFrame = 0;
	yFrame = 0;
	curAnimationTime = animationTimer;
	animationFrameRect.x = 0; animationFrameRect.y = 0;
	animationFrameRect.h = 0; animationFrameRect.w = 0;
}

GraphicsComponent::GraphicsComponent(int animationTimer, std::shared_ptr<TransformComponent> transformComponent)
	: m_animationTimer(animationTimer),
	m_TransformComponent(transformComponent),
	imageOffset(transformComponent->GetSize() / 2)
{
	xFrame = 0;
	yFrame = 0;
	curAnimationTime = animationTimer;
	animationFrameRect.x = 0; animationFrameRect.y = 0;
	animationFrameRect.h = 0; animationFrameRect.w = 0;
}

GraphicsComponent::~GraphicsComponent()
{
	SDL_DestroyTexture(m_sprite);
	m_sprite = NULL;
}

ComponentId GraphicsComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames GraphicsComponent::GetComponentName() const
{
	return EComponentNames::GraphicsComponentEnum;
}

void GraphicsComponent::Update(GameActor& actor, int deltaMs)
{
	/*int spriteWidth, spriteHeight;
	SDL_QueryTexture(m_sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	animationFrameRect.x = xFrame;
	animationFrameRect.y = yFrame;
	animationFrameRect.w = xFrame + m_TransformComponent->GetSize().x;
	animationFrameRect.h = yFrame + m_TransformComponent->GetSize().y;

	if (curAnimationTime < 0)
	{
		curAnimationTime = m_animationTimer;

		xFrame = (int)(xFrame + m_TransformComponent->GetSize().x) % spriteWidth;

		if (xFrame == 0)
			yFrame = (int)(yFrame + m_TransformComponent->GetSize().y) % spriteHeight;
	}
	else
	{
		curAnimationTime--;
	}*/
}