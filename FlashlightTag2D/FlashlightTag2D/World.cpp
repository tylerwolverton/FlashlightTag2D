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
	: m_window(window),
      m_pInputManager(std::make_unique<InputManager>()),
      m_pPhysicsManager(std::make_unique<PhysicsManager>()),
      m_pGraphicsManager(std::make_unique<GraphicsManager>(m_window)),
      m_pActorFactory(std::make_shared<ActorFactory>())
{
    ServiceLocator::Provide(m_pActorFactory);
}

World::~World()
{
}

void World::RunGame()
{
	bool isGameRunning = true;    

    ChangeLevel("resources/levels/level1.json");

    m_pGraphicsManager->AddCamera(m_pActorFactory->CreateCamera());

	auto timeStepMs = 1000.0f / 60; //eg. 60fps
	float timeLastMs = 0;
	float timeCurrentMs = (float)SDL_GetTicks();;
	float timeAccumulatedMs = 0;
	while (isGameRunning)
	{
		auto input = m_pInputManager->ReadInput();
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
			for (auto entity : m_pActorFactory->GetActorList())
			{
				entity->Update(dt, input); // Should this be in this sub loop?
			}

            m_pPhysicsManager->Update(m_pActorFactory->GetActorList(), dt);

			timeAccumulatedMs -= timeStepMs;
		}

        m_pGraphicsManager->Render();
	}
}

void World::ChangeLevel(std::string levelPath)
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
    m_pGraphicsManager->SetBackgroundTexture(d["level"]["sprite"].GetString());

    // loop through actors and call their factory methods 
    // (probably add a generic add actor method to the actor factory that can query for the actor name)
    m_pActorFactory->CreateActorsFromJSONArray(d["actor_list"], *m_pGraphicsManager);
}