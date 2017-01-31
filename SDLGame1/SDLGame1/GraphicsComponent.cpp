#include "GraphicsComponent.h"
#include "GameActor.h"

GraphicsComponent::GraphicsComponent(SDL_Renderer* renderer, int animationTimer)
	: m_renderer(renderer),
	  m_animationTimer(animationTimer)
{
	xFrame = 0;
	yFrame = 0;
	curAnimationTime = animationTimer;
}


GraphicsComponent::~GraphicsComponent()
{
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

void GraphicsComponent::Update(GameActor& actor, int deltaMs)
{
	int spriteWidth, spriteHeight;
	SDL_QueryTexture(actor.m_sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	SDL_Rect imgPartRect;
	imgPartRect.x = actor.m_posX;
	imgPartRect.y = actor.m_posY;
	imgPartRect.w = actor.m_size;
	imgPartRect.h = actor.m_size;

	SDL_Rect locationRect;
	locationRect.x = xFrame;
	locationRect.y = yFrame;
	locationRect.w = xFrame + actor.m_size;
	locationRect.h = yFrame + actor.m_size;

	if (curAnimationTime < 0)
	{
		curAnimationTime = m_animationTimer;

		xFrame = (xFrame + actor.m_size) % spriteWidth;

		if (xFrame == 0)
			yFrame = (yFrame + actor.m_size) % spriteHeight;
	}
	else
	{
		curAnimationTime--;
	}
	//SDL_RenderSetViewport(gRenderer, &topLeftViewport);
	SDL_RenderCopy(m_renderer, actor.m_sprite, &locationRect, &imgPartRect);
}