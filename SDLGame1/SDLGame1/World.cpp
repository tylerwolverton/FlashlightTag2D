#include "World.h"

#include "Vector2D.h"
#include "InputManager.h"
#include "ActorFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"

World::World()
{
	currentCamera = nullptr;
}

World::~World()
{
}

void World::RunGame(SDL_Renderer* renderer)
{
	bool isGameRunning = true;
	auto actorFactory = new ActorFactory(renderer);
	auto inputManager = new InputManager();

	//StrongWorldPtr thisWorld = std::make_shared<World>(*this);
	auto player = AddEntity(actorFactory->CreatePlayer(this));
	AddEntity(actorFactory->CreateEnemy(this));
	AddCamera(actorFactory->CreateCamera(this, player));

	//int tickCount = 0;
	auto timeStepMs = 1000.f / 60; //eg. 30Hz
	float timeLastMs = 0;
	float timeCurrentMs = 0;
	float timeAccumulatedMs = 0;
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

		auto input = inputManager->ReadInput();

		timeLastMs = timeCurrentMs;
		timeCurrentMs = SDL_GetTicks();
		auto timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccumulatedMs += timeDeltaMs;

		while (timeAccumulatedMs >= timeStepMs)
		{
			for (auto entity : entityList)
			{
				entity->Update((int)timeAccumulatedMs, input);
			}

			timeAccumulatedMs -= timeStepMs;
		}

		/*for (auto comp : graphicsComponentList)
		{
			comp->Update(, (int)timeAccumulatedMs);
		}*/

		//Update screen
		SDL_RenderPresent(renderer);
	}
}

void World::AddCamera(StrongGameActorPtr camera)
{
	if (currentCamera == nullptr)
	{
		currentCamera = camera;
	}

	cameraList.push_back(camera);
	entityList.push_back(camera);
}

StrongGameActorPtr World::AddEntity(StrongGameActorPtr entity)
{
	entityList.push_back(entity);
	return entity;
}