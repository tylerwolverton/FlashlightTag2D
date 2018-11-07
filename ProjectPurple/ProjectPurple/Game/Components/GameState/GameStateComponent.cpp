#include "GameStateComponent.h"
#include "HideBehavior.h"

GameStateComponent::GameStateComponent(ComponentId componentId, std::string actorName, std::string actorType)
    : ActorComponent(componentId),
      m_actorName(actorName),
      m_actorType(actorType)
{
}

GameStateComponent::~GameStateComponent()
{
}

// Maybe too much for this component to do? AI could actually select the behavior and this could just set the role
// Player also needs role but not necessarily the same behavior
void GameStateComponent::Update(GameActor& actor, float deltaMs)
{
}

const EComponentNames GameStateComponent::GetComponentName() const
{
    return EComponentNames::GameStateComponentEnum;
}