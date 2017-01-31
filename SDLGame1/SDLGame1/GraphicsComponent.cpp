#include "GraphicsComponent.h"
#include "GameActor.h"

GraphicsComponent::GraphicsComponent(SDL_Renderer* renderer)
	: m_renderer(renderer)
{
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
	SDL_Rect imgPartRect;
	imgPartRect.x = actor.m_posX;
	imgPartRect.y = actor.m_posY;
	imgPartRect.w = 100;
	imgPartRect.h = 100;

	SDL_Rect locationRect;
	locationRect.x = 0;
	locationRect.y = 0;
	locationRect.w = 100;
	locationRect.h = 100;

	//SDL_RenderSetViewport(gRenderer, &topLeftViewport);
	SDL_RenderCopy(m_renderer, actor.m_sprite, &locationRect, &imgPartRect);
}