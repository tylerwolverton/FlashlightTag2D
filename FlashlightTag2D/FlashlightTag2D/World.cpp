#include "World.h"

#include "Vector2D.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

World::World(SDL_Window* window)
	: m_window(window)
{
	m_pCurrentCamera = nullptr;
}

World::~World()
{
}

void World::RunGame()
{
	bool isGameRunning = true;
	auto actorFactory = new ActorFactory();
	auto inputManager = new InputManager();
	auto physicsManager = new PhysicsManager();
	auto graphicsManager = new GraphicsManager(m_window);

	//StrongWorldPtr thisWorld = std::make_shared<World>(*this);
	auto player = AddEntity(actorFactory->CreatePlayer());
	AddEntity(actorFactory->CreateEnemy());
	AddCamera(actorFactory->CreateCamera(player));

	auto timeStepMs = 1000.f / 60; //eg. 30Hz
	float timeLastMs = 0;
	float timeCurrentMs = 0;
	float timeAccumulatedMs = 0;
	while (isGameRunning)
	{
		auto input = inputManager->ReadInput();
		if (input & EInputValues::Esc)
		{
			isGameRunning = false;
			break;
		}

		timeLastMs = timeCurrentMs;
		timeCurrentMs = SDL_GetTicks();
		auto timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccumulatedMs += timeDeltaMs;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		while (timeAccumulatedMs >= timeStepMs)
		{
			for (auto entity : m_pEntityList)
			{
				entity->Update((int)timeAccumulatedMs, input);
			}

			physicsManager->ResolveCollisions(m_pEntityList);

			timeAccumulatedMs -= timeStepMs;
		}

		//graphicsManager->RenderBackground(backgroundSprite, GetCurrentCamera(), renderer, LEVEL_WIDTH, LEVEL_HEIGHT);
		graphicsManager->Render(m_pEntityList, GetCurrentCamera());
	}
}

void World::AddCamera(StrongGameActorPtr camera)
{
	if (m_pCurrentCamera == nullptr)
	{
		m_pCurrentCamera = camera;
	}

	m_pCameraList.push_back(camera);
	m_pEntityList.push_back(camera);
}

StrongGameActorPtr World::AddEntity(StrongGameActorPtr entity)
{
	m_pEntityList.push_back(entity);
	return entity;
}