#include <memory>

#include "World.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "ServiceLocator.h"
#include "GameStateComponent.h"
#include "GameTypes.h"

// from rapidjson
#include "document.h"
#include "filereadstream.h"

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
	auto actorFactory = std::make_shared<ActorFactory>();
	ServiceLocator::Provide(actorFactory);

	auto inputManager = std::make_shared<InputManager>();
	auto physicsManager = std::make_shared<PhysicsManager>();
	auto graphicsManager = std::make_shared<GraphicsManager>(m_window);
    

    ChangeLevel("resources/levels/level1.json", graphicsManager, actorFactory);

	AddCamera(actorFactory->CreateCamera());

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

void World::ChangeLevel(std::string levelPath, StrongGraphicsManagerPtr graphicsManager, StrongActorFactoryPtr actorFactory)
{
    FILE* fp;
    fopen_s(&fp, levelPath.c_str(), "rb");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document d;
    d.ParseStream(is);

    fclose(fp);

    // set level height and width
    m_levelSize = Vector2D<int>(d["level"]["size"]["x"].GetInt(), d["level"]["size"]["y"].GetInt());

    // set graphicsManager::backgroundSprite somehow
    graphicsManager->SetBackgroundTexture(d["level"]["sprite"].GetString());

    // loop through actors and call their factory methods 
    // (probably add a generic add actor method to the actor factory that can query for the actor name)
    actorFactory->CreateActorsFromJSONArray(d["actor_list"]);
}