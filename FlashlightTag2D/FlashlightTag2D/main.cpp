#include <stdio.h>
#include <string>

#include <SDL.h>
#include "World.h"
#include "ServiceLocator.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

// Our opengl context handle
SDL_GLContext mainContext;

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
	gWindow = SDL_CreateWindow("SDLGame1", 
							   SDL_WINDOWPOS_UNDEFINED,
							   SDL_WINDOWPOS_UNDEFINED, 
							   World::SCREEN_WIDTH, 
							   World::SCREEN_HEIGHT,
							   SDL_WINDOW_OPENGL);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(gWindow);

    SDL_ShowCursor(SDL_DISABLE);

	return true;
}

//Frees media and shuts down SDL
void close()
{
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
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

	std::shared_ptr<World> world = std::make_shared<World>(gWindow);

	ServiceLocator::Provide(world);
	world->RunGame();

	close();
	return 0;
}