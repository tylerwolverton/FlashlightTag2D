#include "PortalLogicComponent.h"
#include "LevelFactory.h"
#include "ServiceLocator.h"

PortalLogicComponent::PortalLogicComponent(ComponentId componentId, std::string levelName)
    : LogicComponent(componentId),
      m_levelName(levelName)
{
}

PortalLogicComponent::~PortalLogicComponent()
{
}

void PortalLogicComponent::ChangeLevel()
{
    std::shared_ptr<LevelFactory> levelFactory = ServiceLocator::GetLevelFactory();

    if (levelFactory != nullptr)
    {
        levelFactory->ChangeLevel(m_levelName);
    }
}