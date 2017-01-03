/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

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
SDL_Texture* gTexture = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

//Current displayed image
SDL_Surface* gPNGSurface = NULL;

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
			
	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	return true;
}

uint32_t handleInput();

SDL_Surface* loadBMPSurface(std::string path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}

	//Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return optimizedSurface;
}

SDL_Surface* loadPNGSurface(std::string path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}

	//Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return optimizedSurface;
}

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
	gPNGSurface = loadPNGSurface("resources/loaded.png");
	if (gPNGSurface == NULL)
	{
		printf("Failed to load png image!\n");
		return false;
	}

	//Load PNG texture
	gTexture = loadTexture("resources/texture.png");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		return false;
	}

	return true;
}

//Frees media and shuts down SDL
void close()
{
	//Deallocate surface
	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;

	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

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
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			close();

			return 1;
		}

		uint32_t buttonState = 0;
		while (buttonState != UINT32_MAX)
		{
			buttonState = handleInput();

			//Apply the image
			//SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

			//Update the surface
			//SDL_UpdateWindowSurface(gWindow);

			//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			//Top left corner viewport
			SDL_Rect topLeftViewport;
			topLeftViewport.x = 0;
			topLeftViewport.y = 0;
			topLeftViewport.w = SCREEN_WIDTH / 2;
			topLeftViewport.h = SCREEN_HEIGHT / 2;
			SDL_RenderSetViewport(gRenderer, &topLeftViewport);

			//Render texture to screen
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			//Top right corner viewport
			SDL_Rect topRightViewport;
			topRightViewport.x = SCREEN_WIDTH / 2;
			topRightViewport.y = 0;
			topRightViewport.w = SCREEN_WIDTH / 2;
			topRightViewport.h = SCREEN_HEIGHT / 2;
			SDL_RenderSetViewport(gRenderer, &topRightViewport);

			//Render texture to screen
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			SDL_Rect bottomViewport;
			bottomViewport.x = 0;
			bottomViewport.y = SCREEN_HEIGHT / 2;
			bottomViewport.w = SCREEN_WIDTH;
			bottomViewport.h = SCREEN_HEIGHT / 2;
			SDL_RenderSetViewport(gRenderer, &bottomViewport);

			//Render texture to screen
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			SDL_Rect minimapViewport;
			minimapViewport.x = SCREEN_WIDTH - 150;
			minimapViewport.y = SCREEN_HEIGHT - 150;
			minimapViewport.w = 150;
			minimapViewport.h = 150;
			SDL_RenderSetViewport(gRenderer, &minimapViewport);

			//Render texture to screen
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			////Render red filled quad
			//SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
			//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
			//SDL_RenderFillRect(gRenderer, &fillRect);

			////Render green outlined quad
			//SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
			//SDL_RenderDrawRect(gRenderer, &outlineRect);

			////Draw blue horizontal line
			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
			//SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

			////Draw vertical line of yellow dots
			//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
			//for (int i = 0; i < SCREEN_HEIGHT; i += 4)
			//{
			//	SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
			//}

			//Render texture to screen
			//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

			//Update screen
			SDL_RenderPresent(gRenderer);
		}
	}

	close();

	return 0;
}

// Returns
uint32_t handleInput()
{
	uint32_t buttonState = 0;

	//Handle events on queue
	// SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			return UINT32_MAX;
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
			buttonState = UINT32_MAX;

		return buttonState;
	}
}