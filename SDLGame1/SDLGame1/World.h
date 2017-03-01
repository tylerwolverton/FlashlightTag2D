#pragma once
#include <memory>
#include <SDL.h>
#include "Types.h"

#define MAX_CAMERAS 10

class World
{
public:
	//The dimensions of the level
	static const int LEVEL_WIDTH = 1280;
	static const int LEVEL_HEIGHT = 960;

	//Screen dimension constants
	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;

	StrongGameActorPtrList entityList;
	StrongGameActorPtrList cameraList;
	ComponentList graphicsComponentList; 
	SDL_Renderer* renderer;

	World(SDL_Renderer* renderer);
	virtual ~World();

	void RunGame();
	void AddCamera(StrongGameActorPtr camera);
	StrongGameActorPtr AddEntity(StrongGameActorPtr entity);

	StrongGameActorPtr GetCurrentCamera() { return currentCamera; };

private:
	StrongGameActorPtr currentCamera;
};

