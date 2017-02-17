#include "World.h"

#include "Vector2D.h"
#include "ActorFactory.h"
#include "GameActor.h"


World::World()
{
	numCameras = 0;
	curCamera = -1;
}


World::~World()
{
}

void World::RunGame(SDL_Renderer* renderer)
{
	bool isGameRunning = true;
	auto actorFactory = new ActorFactory(renderer);

	StrongWorldPtr thisWorld = std::make_shared<World>(*this);
	auto player = actorFactory->CreatePlayer(thisWorld);
	auto enemy = actorFactory->CreateEnemy(thisWorld);
	auto camera = actorFactory->CreateCamera(thisWorld, player);
	thisWorld->AddCamera(camera);

	int tickCount = 0;
	while (isGameRunning)
	{
		//Clear screen
		SDL_RenderClear(renderer);

		//Top left corner viewport
		SDL_Rect topLeftViewport;
		topLeftViewport.x = 0;
		topLeftViewport.y = 0;
		topLeftViewport.w = SCREEN_WIDTH;
		topLeftViewport.h = SCREEN_HEIGHT;
		SDL_RenderSetViewport(renderer, &topLeftViewport);

		player->Update(tickCount);
		enemy->Update(tickCount);
		camera->Update(tickCount);
		tickCount = 60;

		//Update screen
		SDL_RenderPresent(renderer);
	}
}

void World::AddCamera(StrongGameActorPtr camera)
{
	if (numCameras + 1 > MAX_CAMERAS)
	{
		return;
	}

	cameraList[numCameras] = camera;

	numCameras++;

	if (curCamera < 0)
		curCamera = 0;
}