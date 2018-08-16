#include <memory>

#include "World.h"
#include "Level.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "ServiceLocator.h"
#include "GameStateComponent.h"
#include "GameTypes.h"
#include "RandomNumberGenerator.h"

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
	ServiceLocator::Provide(std::make_shared<RandomNumberGenerator>());
}

World::~World()
{
}

void World::RunGame()
{
	bool isGameRunning = true;    

    //ChangeLevel("resources/levels/main_menu.json");
	ChangeLevel("resources/levels/level1.json");
	changeGameMode();

    m_pGraphicsManager->AddCamera(m_pActorFactory->CreateCamera(m_curLevel->GetLevelSize()));

	float timeStepMs = 1000.0f / 60; //eg. 60fps
	float timeLastMs = 0;
	float timeCurrentMs = (float)SDL_GetTicks();
	float timeAccumulatedMs = 0;
	while (isGameRunning)
	{
		uint32_t input = m_pInputManager->ReadInput();
		if (input & EInputValues::Esc)
		{
			isGameRunning = false;
			break;
		}

		timeLastMs = timeCurrentMs;
		timeCurrentMs = (float)SDL_GetTicks();
		float timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccumulatedMs += timeDeltaMs;
		while (timeAccumulatedMs >= timeStepMs)
		{
            auto dt = timeAccumulatedMs / 1000;
			for (std::shared_ptr<GameActor> entity : m_pActorFactory->GetActorList())
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
	int levelWidth = d["level"]["size"]["x"].GetInt();
	int levelHeight = d["level"]["size"]["y"].GetInt();
	std::string sprite = d["level"]["sprite"].GetString();
	std::string vertShader = d["level"]["vert_shader"].GetString();
	std::string fragShader = d["level"]["frag_shader"].GetString();

	m_curLevel = std::make_shared<Level>(levelWidth, levelHeight, sprite, vertShader, fragShader);

    m_pPhysicsManager->SetLevelSize(m_curLevel->GetLevelSize());

    m_pGraphicsManager->LoadNewLevel(m_curLevel);

    m_pActorFactory->CreateActorsFromJSONArray(d["actor_list"], *m_pPhysicsManager, *m_pGraphicsManager, m_curLevel);
}

void World::changeGameMode()
{
	m_pActorFactory->ChooseSeekers(1);
}