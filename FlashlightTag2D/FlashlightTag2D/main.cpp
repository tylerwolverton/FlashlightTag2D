#include <tinyxml2.h>
#include <stdio.h>
#include <string>
#include "World.h"
#include "GraphicsComponent.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gBackgroundTexture = NULL;

//Starts up SDL and creates window
bool init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("SDLGame1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, World::SCREEN_WIDTH, World::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
			
	return true;
}

//Loads media
bool loadMedia()
{
	//Load PNG texture
	gBackgroundTexture = LoadTexture(gRenderer, "resources/background.png");
	if (gBackgroundTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		return false;
	}

	return true;
}

//Frees media and shuts down SDL
void close()
{
	SDL_DestroyTexture(gBackgroundTexture);
	gBackgroundTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{

	if (!init())
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		close();
		return 1;
	}

	//Load media
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
		close();
		return 1;
	}

	auto world = new World(gRenderer);
	world->RunGame();

	close();
	return 0;
}