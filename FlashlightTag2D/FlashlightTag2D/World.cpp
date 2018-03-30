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
      m_pInputManager(std::make_shared<InputManager>()),
      m_pPhysicsManager(std::make_shared<PhysicsManager>()),
      m_pGraphicsManager(std::make_shared<GraphicsManager>(m_window)),
      m_pActorFactory(std::make_shared<ActorFactory>())
{
    ServiceLocator::Provide(m_pActorFactory);
	ServiceLocator::Provide(m_pInputManager);
	ServiceLocator::Provide(m_pPhysicsManager);
	ServiceLocator::Provide(m_pGraphicsManager);
}

World::~World()
{
}

void World::RunGame()
{
	bool isGameRunning = true;    

    ChangeLevel("resources/levels/level1.json");
	changeGameMode();

    m_pGraphicsManager->AddCamera(m_pActorFactory->CreateCamera());

	auto timeStepMs = 1000.0f / 60; //eg. 60fps
	float timeLastMs = 0;
	float timeCurrentMs = (float)SDL_GetTicks();
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

            m_pPhysicsManager->Update(dt);

			timeAccumulatedMs -= timeStepMs;
		}

        m_pGraphicsManager->Render();
	}
}

void World::ChangeLevel(const std::string& levelPath)
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

    m_pGraphicsManager->SetBackgroundTexture(d["level"]["sprite"].GetString());

    m_pActorFactory->CreateActorsFromJSONArray(d["actor_list"], *m_pPhysicsManager, *m_pGraphicsManager);
}

void World::changeGameMode()
{
	m_pActorFactory->ChooseSeekers(1);
}