#include "World.h"

#include "Vector2D.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "ServiceLocator.h"
#include "GameStateComponent.h"
#include "GameTypes.h"

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
	ServiceLocator::Provide(actorFactory);

	auto inputManager = new InputManager();
	auto physicsManager = new PhysicsManager();
	auto graphicsManager = new GraphicsManager(m_window);

	auto player = actorFactory->CreatePlayer();
	actorFactory->CreateEnemy(Vector2D<float>(200.0f, 100.0f), EGameRole::Seeker);
	actorFactory->CreateEnemy(Vector2D<float>(700.0f, 400.0f), EGameRole::Hider);
	AddCamera(actorFactory->CreateCamera(player));

	auto timeStepMs = 1000.0f / 60; //eg. 60fps
	float timeLastMs = 0;
	float timeCurrentMs = (float)SDL_GetTicks();;
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
		timeCurrentMs = (float)SDL_GetTicks();
		auto timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccumulatedMs += timeDeltaMs;
		while (timeAccumulatedMs >= timeStepMs)
		{
            auto dt = timeAccumulatedMs / 1000;
			for (auto entity : actorFactory->GetActorList())
			{
				entity->Update(dt, input); // Should this be in this sub loop?
			}

			physicsManager->Update(actorFactory->GetActorList(), dt);

			timeAccumulatedMs -= timeStepMs;
		}

		graphicsManager->Render(actorFactory->GetActorList(), GetCurrentCamera());
	}
}

void World::AddCamera(StrongGameActorPtr camera)
{
	if (m_pCurrentCamera == nullptr)
	{
		m_pCurrentCamera = camera;
	}

	m_pCameraList.push_back(camera);
}