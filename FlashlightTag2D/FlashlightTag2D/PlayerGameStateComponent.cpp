#include "PlayerGameStateComponent.h"

PlayerGameStateComponent::PlayerGameStateComponent(ComponentId componentId, std::string actorName)
    : GameStateComponent(componentId, actorName),
      m_hasKey(false)
{
}

PlayerGameStateComponent::~PlayerGameStateComponent()
{
}
