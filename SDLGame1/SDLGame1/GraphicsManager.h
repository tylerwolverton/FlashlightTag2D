#pragma once
#include "Types.h"

#include <SDL.h>
class GraphicsManager
{
public:
	//SDL_Renderer* m_renderer;

	GraphicsManager();
	~GraphicsManager();

	void Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera, SDL_Renderer* renderer);
};

