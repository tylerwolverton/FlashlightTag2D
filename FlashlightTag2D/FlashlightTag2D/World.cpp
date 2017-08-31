#include "World.h"
//#include <glew.h>

#include "Vector2D.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"

//World::World(SDL_Renderer* p_renderer)
//	: renderer(p_renderer)
//{
//	currentCamera = nullptr;
//}

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

	/*auto backgroundSprite = LoadTexture(m_renderer, "resources/background.png");
	if (backgroundSprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}*/

	auto timeStepMs = 1000.f / 60; //eg. 30Hz
	float timeLastMs = 0;
	float timeCurrentMs = 0;
	float timeAccumulatedMs = 0;
	while (isGameRunning)
	{
		//Clear screen
		//graphicsManager->ClearScreen();
		//SDL_RenderClear(m_renderer);

		//Top left corner viewport
		/*SDL_Rect topLeftViewport;
		topLeftViewport.x = 0;
		topLeftViewport.y = 0;
		topLeftViewport.w = SCREEN_WIDTH;
		topLeftViewport.h = SCREEN_HEIGHT;

		SDL_RenderSetViewport(m_renderer, &topLeftViewport);*/

		auto input = inputManager->ReadInput();

		timeLastMs = timeCurrentMs;
		timeCurrentMs = SDL_GetTicks();
		auto timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccumulatedMs += timeDeltaMs;

		while (timeAccumulatedMs >= timeStepMs)
		{
			for (auto entity : m_pEntityList)
			{
				entity->Update((int)timeAccumulatedMs, input);
			}

			physicsManager->ResolveCollisions(m_pEntityList);

			timeAccumulatedMs -= timeStepMs;
		}

		//graphicsManager->ClearScreen();
		//graphicsManager->RenderBackground(backgroundSprite, GetCurrentCamera(), renderer, LEVEL_WIDTH, LEVEL_HEIGHT);
		graphicsManager->Render(m_pEntityList, GetCurrentCamera());

		//Update screen
		//SDL_RenderPresent(renderer);
		SDL_GL_SwapWindow(m_window);
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