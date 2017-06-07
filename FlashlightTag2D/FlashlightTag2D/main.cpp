#include <tinyxml2.h>
#include <stdio.h>
#include <string>
#include "World.h"
#include "GraphicsComponent.h"

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <glew.h>

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gBackgroundTexture = NULL;

// Our opengl context handle
SDL_GLContext mainContext;

//bool SetOpenGLAttributes()
//{
//	// Set our OpenGL version.
//	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//
//	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//
//	// Turn on double buffering with a 24bit Z buffer.
//	// You may need to change this to 16 or 32 for your system
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//
//	return true;
//}

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

	/*SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	*/
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
//bool loadMedia()
//{
//	//Load PNG texture
//	gBackgroundTexture = loadTexture(gRenderer, "resources/background.png");
//	if (gBackgroundTexture == NULL)
//	{
//		printf("Failed to load texture image!\n");
//		return false;
//	}
//
//	return true;
//}

//Frees media and shuts down SDL
void close()
{
	SDL_DestroyTexture(gBackgroundTexture);
	gBackgroundTexture = NULL;

	//Destroy window
	//SDL_DestroyRenderer(gRenderer);
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
	//if (!loadMedia())
	//{
	//	printf("Failed to load media!\n");
	//	close();
	//	return 1;
	//}

	auto world = new World(gWindow);
	world->RunGame();

	close();
	return 0;
}