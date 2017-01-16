#include <SDL.h>
#include <SDL_image.h>
#include <tinyxml2.h>
#include <stdio.h>
#include <string>
#include "Types.h"
#include "Actor.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum EInputValues
{
	UP = 0x0001,
	DOWN = 0x002,
	LEFT = 0x004,
	RIGHT = 0x0008
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gBackgroundTexture = NULL;
SDL_Texture* gSpriteTexture = NULL;

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
	gWindow = SDL_CreateWindow("SDLGame1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

bool handleInput(uint32_t& buttonState);
void ProcessInput(uint32_t buttonState, int& xPos, int& yPos);

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

//Loads media
bool loadMedia()
{
	//Load PNG texture
	gBackgroundTexture = loadTexture("resources/background.png");
	if (gBackgroundTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		return false;
	}

	//Load PNG texture
	gSpriteTexture = loadTexture("resources/spritesheet.png");
	if (gSpriteTexture == NULL)
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
	SDL_DestroyTexture(gSpriteTexture);
	gBackgroundTexture = NULL;
	gSpriteTexture = NULL;

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

	uint32_t buttonState = 0;
	bool isGameRunning = true;
	int xPos = 0, yPos = 0;
	int tickCount = 0;
	while (isGameRunning)
	{
		isGameRunning = handleInput(buttonState);
		
		if (--tickCount <= 0)
		{
			ProcessInput(buttonState, xPos, yPos);
			tickCount = 60;
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Top left corner viewport
		SDL_Rect topLeftViewport;
		topLeftViewport.x = 0;
		topLeftViewport.y = 0;
		topLeftViewport.w = SCREEN_WIDTH;
		topLeftViewport.h = SCREEN_HEIGHT;
		SDL_RenderSetViewport(gRenderer, &topLeftViewport);

		//Render texture to screen
		SDL_RenderCopy(gRenderer, gBackgroundTexture, NULL, NULL);

		SDL_Rect imgPartRect;
		imgPartRect.x = xPos;
		imgPartRect.y = yPos;
		imgPartRect.w = 100;
		imgPartRect.h = 100;

		SDL_Rect locationRect;
		locationRect.x = 0;
		locationRect.y = 0;
		locationRect.w = 100;
		locationRect.h = 100;

		//SDL_RenderSetViewport(gRenderer, &topLeftViewport);
		SDL_RenderCopy(gRenderer, gSpriteTexture, &locationRect, &imgPartRect);

		//
		//SDL_Rect minimapViewport;
		//minimapViewport.x = SCREEN_WIDTH - 150;
		//minimapViewport.y = SCREEN_HEIGHT - 150;
		//minimapViewport.w = 150;
		//minimapViewport.h = 150;
		//SDL_RenderSetViewport(gRenderer, &minimapViewport);

		////Render texture to screen
		//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}

// Returns
bool handleInput(uint32_t& buttonState)
{
	//Handle events on queue
	// SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			return false;
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_W])
		{
			buttonState |= 1 << 1;
		}
		else
		{
			buttonState &= ~(1 << 1);
		}
		if (keys[SDL_SCANCODE_A])
		{
			buttonState |= 1 << 2;
		}
		else
		{
			buttonState &= ~(1 << 2);
		}
		if (keys[SDL_SCANCODE_S])
			buttonState |= 1 << 3;
		else
		{
			buttonState &= ~(1 << 3);
		}
		if (keys[SDL_SCANCODE_D])
		{
			buttonState |= 1 << 4;
		}
		else
		{
			buttonState &= ~(1 << 4);
		}
		if (keys[SDL_SCANCODE_ESCAPE])
			return false;
	}

	return true;
}

void ProcessInput(uint32_t buttonState, int& xPos, int& yPos)
{
	if ((buttonState >> 1) & 1)
	{
		// W was pressed
		yPos -= 1;
	}
	if ((buttonState >> 2) & 1)
	{
		// A was pressed
		xPos -= 1;
	}
	if ((buttonState >> 3) & 1)
	{
		// S was pressed
		yPos += 1;
	}
	if ((buttonState >> 4) & 1)
	{
		// D was pressed
		xPos += 1;
	}
}