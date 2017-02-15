#pragma once
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

	SDL_Rect cameraList[MAX_CAMERAS];

	World();
	virtual ~World();

	void RunGame(SDL_Renderer* renderer);

private:
	int curCamera;
	int numCameras;
};

