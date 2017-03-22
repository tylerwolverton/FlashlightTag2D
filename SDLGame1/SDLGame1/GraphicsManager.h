#pragma once
#include "Types.h"

#include <SDL.h>
//#include <SDL_image.h>

class GraphicsManager
{
public:
	//SDL_Renderer* m_renderer;

	GraphicsManager();
	~GraphicsManager();

	void Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera, SDL_Renderer* renderer);
	void GraphicsManager::RenderBackground(SDL_Texture* sprite, StrongGameActorPtr currentCamera, SDL_Renderer* renderer, int screenWidth, int screenHeight);
};

