#include "World.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "ActorFactory.h"
#include "LevelFactory.h"
#include "GameActor.h"
#include "ServiceLocator.h"
#include "RandomNumberGenerator.h"

World::World(SDL_Window* window)
    : m_escHoldTime(0),
	  m_inputManagerPtr(std::make_shared<InputManager>()),
	  m_physicsManagerPtr(std::make_shared<PhysicsManager>()),
	  m_graphicsManagerPtr(std::make_shared<GraphicsManager>(window)),
	  m_actorFactoryPtr(std::make_shared<ActorFactory>(m_physicsManagerPtr, m_graphicsManagerPtr)),
	  m_levelFactoryPtr(std::make_shared<LevelFactory>(m_actorFactoryPtr))
{
    ServiceLocator::Provide(m_actorFactoryPtr);
    ServiceLocator::Provide(m_levelFactoryPtr);
    ServiceLocator::Provide(m_graphicsManagerPtr);
    ServiceLocator::Provide(std::make_shared<RandomNumberGenerator>());
}

World::~World()
{
}

void World::RunGame()
{
    m_isGameRunning = true;    

	m_levelFactoryPtr->ChangeLevel(LevelFactory::LevelPaths::MainMenu);

    float timeStepMs = 1000.0f / 60; //eg. 60fps
    float timeLastMs = 0;
    float timeCurrentMs = (float)SDL_GetTicks();
    float timeAccumulatedMs = 0;
    while (m_isGameRunning)
    {
        InputData input = m_inputManagerPtr->ReadInput();
        
        timeLastMs = timeCurrentMs;
        timeCurrentMs = (float)SDL_GetTicks();
        float timeDeltaMs = timeCurrentMs - timeLastMs;

        if (input.buttonsPressed & EInputValues::Esc)
        {
            m_escHoldTime += timeDeltaMs;
        }
        else
        {
            m_escHoldTime = 0;
        }
        
        if (input.buttonsPressed & EInputValues::Quit
            || m_escHoldTime > 2 * 1000)
        {
            m_isGameRunning = false;
            break;
        }

        timeAccumulatedMs += timeDeltaMs;
        while (timeAccumulatedMs >= timeStepMs)
        {
            auto dt = timeAccumulatedMs / 1000;
            for (auto entity : m_actorFactoryPtr->GetActorMap())
            {
                entity.second->Update(dt, input); // Should this be in this sub loop?
                //m_pLevelFactory->UpdateLevelTilesForActor(entity.second);
            }

			m_physicsManagerPtr->Update(dt);

            timeAccumulatedMs -= timeStepMs;
        }

		m_graphicsManagerPtr->Render();

		m_actorFactoryPtr->RemoveDeadActors();
    }
}

void World::QuitGame()
{
    m_isGameRunning = false;
}