#include "World.h"

#include "Vector2D.h"
#include "ActorFactory.h"
#include "GameActor.h"


World::World()
{
}


World::~World()
{
}

void World::RunGame(SDL_Renderer* renderer)
{
	bool isGameRunning = true;
	auto actorFactory = new ActorFactory(renderer);
	auto player = actorFactory->CreatePlayer();
	auto enemy = actorFactory->CreateEnemy();
	int tickCount = 0;
	while (isGameRunning)
	{
		//Clear screen
		SDL_RenderClear(renderer);

		//The camera area
		SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		//Top left corner viewport
		SDL_Rect topLeftViewport;
		topLeftViewport.x = 0;
		topLeftViewport.y = 0;
		topLeftViewport.w = SCREEN_WIDTH;
		topLeftViewport.h = SCREEN_HEIGHT;
		SDL_RenderSetViewport(renderer, &topLeftViewport);

		//Render texture to screen
		//SDL_RenderCopy(renderer, gBackgroundTexture, NULL, NULL);

		player->Update(tickCount);
		enemy->Update(tickCount);
		tickCount = 60;

		//Center the camera over the dot
		camera.x = (player->m_position.x + player->m_size.x / 2) - SCREEN_WIDTH / 2;
		camera.y = (player->m_position.y + player->m_size.y / 2) - SCREEN_HEIGHT / 2;

		//Keep the camera in bounds
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w)
		{
			camera.x = LEVEL_WIDTH - camera.w;
		}
		if (camera.y > LEVEL_HEIGHT - camera.h)
		{
			camera.y = LEVEL_HEIGHT - camera.h;
		}

		//Update screen
		SDL_RenderPresent(renderer);
	}
}