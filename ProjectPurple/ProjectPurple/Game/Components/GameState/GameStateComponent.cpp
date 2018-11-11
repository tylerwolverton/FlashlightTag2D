#include "GameStateComponent.h"

GameStateComponent::GameStateComponent(ComponentId componentId, std::string actorName, std::string actorType)
    : ActorComponent(componentId),
      m_actorName(actorName),
      m_actorType(actorType)
{
}

GameStateComponent::~GameStateComponent()
{
}

void GameStateComponent::Update(GameActor& actor, float deltaMs)
{
}

const EComponentNames GameStateComponent::GetComponentName() const
{
    return EComponentNames::GameStateComponentEnum;
}