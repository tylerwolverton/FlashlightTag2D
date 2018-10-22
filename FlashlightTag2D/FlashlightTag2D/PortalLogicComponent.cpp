#include "PortalLogicComponent.h"
#include "LevelFactory.h"
#include "ServiceLocator.h"

#include <SDL.h>

PortalLogicComponent::PortalLogicComponent(ComponentId componentId, std::string levelName, float countdownTimeSec)
    : LogicComponent(componentId),
      m_levelName(levelName),
      m_countdownTimeSec(countdownTimeSec)
{
    m_firstTickVal = SDL_GetTicks();
}

PortalLogicComponent::~PortalLogicComponent()
{
}

void PortalLogicComponent::Update(GameActor& actor, float deltaMs)
{
    if (m_countdownTimeSec > 0)
    {
        uint32_t curTicks = SDL_GetTicks();
        if (curTicks - m_firstTickVal > m_countdownTimeSec * 1000)
        {
            ChangeLevel();
        }
    }
}

void PortalLogicComponent::ChangeLevel()
{
    std::shared_ptr<LevelFactory> levelFactory = ServiceLocator::GetLevelFactory();

    if (levelFactory != nullptr)
    {
        levelFactory->ChangeLevel(m_levelName);
    }
}