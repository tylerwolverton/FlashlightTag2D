#include "PlayerGameStateComponent.h"

PlayerGameStateComponent::PlayerGameStateComponent(ComponentId componentId, std::string actorName)
    : GameStateComponent(componentId, actorName),
	  m_inventory(std::map<ActorId, std::shared_ptr<GameActor>>()),
      m_hasKey(false)
{
}

PlayerGameStateComponent::~PlayerGameStateComponent()
{
}
