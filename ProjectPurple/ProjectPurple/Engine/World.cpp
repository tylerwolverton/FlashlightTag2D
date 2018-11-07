#include <memory>

#include "World.h"
#include "Level.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "LevelFactory.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "ServiceLocator.h"
#include "GameStateComponent.h"
#include "GameTypes.h"
#include "RandomNumberGenerator.h"

World::World(SDL_Window* window)
    : m_window(window),
      m_isGamePaused(false),
      m_pInputManager(std::make_shared<InputManager>()),
      m_pPhysicsManager(std::make_shared<PhysicsManager>()),
      m_pGraphicsManager(std::make_shared<GraphicsManager>(m_window)),
      m_pActorFactory(std::make_shared<ActorFactory>(m_pPhysicsManager, m_pGraphicsManager)),
      m_pLevelFactory(std::make_shared<LevelFactory>(m_pActorFactory))
{
    ServiceLocator::Provide(m_pActorFactory);
    ServiceLocator::Provide(m_pLevelFactory);
    ServiceLocator::Provide(m_pGraphicsManager);
    ServiceLocator::Provide(std::make_shared<RandomNumberGenerator>());
}

World::~World()
{
}

void World::RunGame()
{
    m_isGameRunning = true;    

    m_pLevelFactory->ChangeLevel(LevelFactory::LevelPaths::MainMenu);
    changeGameMode();

    float timeStepMs = 1000.0f / 60; //eg. 60fps
    float timeLastMs = 0;
    float timeCurrentMs = (float)SDL_GetTicks();
    float timeAccumulatedMs = 0;
    while (m_isGameRunning)
    {
        InputData input = m_pInputManager->ReadInput();
        if (input.buttonsPressed & EInputValues::Esc)
        {
            m_isGameRunning = false;
            break;
        }

        timeLastMs = timeCurrentMs;
        timeCurrentMs = (float)SDL_GetTicks();
        float timeDeltaMs = timeCurrentMs - timeLastMs;
        timeAccumulatedMs += timeDeltaMs;
        while (timeAccumulatedMs >= timeStepMs)
        {
            auto dt = timeAccumulatedMs / 1000;
            for (auto entity : m_pActorFactory->GetActorMap())
            {
                entity.second->Update(dt, input); // Should this be in this sub loop?
                //m_pLevelFactory->UpdateLevelTilesForActor(entity.second);
            }

            m_pPhysicsManager->Update(dt);

            timeAccumulatedMs -= timeStepMs;
        }

        m_pGraphicsManager->Render();

        m_pActorFactory->RemoveDeadActors();
    }
}

void World::QuitGame()
{
    m_isGameRunning = false;
}

void World::changeGameMode()
{
}