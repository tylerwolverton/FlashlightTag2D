#include "GraphicsComponent.h"
#include "GameActor.h"
#include "World.h"

GraphicsComponent::GraphicsComponent(SDL_Texture* sprite, int animationTimer)
	: m_animationTimer(animationTimer)
{
	xFrame = 0;
	yFrame = 0;
	m_sprite = sprite;
	curAnimationTime = animationTimer;
	animationFrameRect.x = 0; animationFrameRect.y = 0;
	animationFrameRect.h = 0; animationFrameRect.w = 0;
}

GraphicsComponent::~GraphicsComponent()
{
	SDL_DestroyTexture(m_sprite);
	m_sprite = NULL;
}

bool GraphicsComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
}

void GraphicsComponent::PostInit()
{

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
	int spriteWidth, spriteHeight;
	SDL_QueryTexture(m_sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	animationFrameRect.x = xFrame;
	animationFrameRect.y = yFrame;
	animationFrameRect.w = xFrame + actor.m_size.x;
	animationFrameRect.h = yFrame + actor.m_size.y;

	if (curAnimationTime < 0)
	{
		curAnimationTime = m_animationTimer;

		xFrame = (xFrame + actor.m_size.x) % spriteWidth;

		if (xFrame == 0)
			yFrame = (yFrame + actor.m_size.y) % spriteHeight;
	}
	else
	{
		curAnimationTime--;
	}
}