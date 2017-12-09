#include "GameStateComponent.h"
#include "HideBehavior.h"
#include "SeekBehavior.h"

GameStateComponent::GameStateComponent(std::string actorName, EGameRole role)
	: m_curRole(role),
	  m_prevRole(role),
	  m_actorName(actorName)
{
	updateBehavior();
}

GameStateComponent::~GameStateComponent()
{
}

void GameStateComponent::Update(GameActor& actor, int deltaMs)
{
	// Check for a role change and update behavior
	if (m_curRole != m_prevRole)
	{
		updateBehavior();

		m_prevRole = m_curRole;
	}
}

void GameStateComponent::updateBehavior()
{
	switch (m_curRole)
	{
		case EGameRole::Hider:
			m_behavior = std::make_shared<HideBehavior>();
			break;
		case EGameRole::Seeker:
			m_behavior = std::make_shared<SeekBehavior>();
			break;
		case EGameRole::Out:
			break;
		default:
			break;
	}
}

ComponentId GameStateComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames GameStateComponent::GetComponentName() const
{
	return EComponentNames::GameStateComponentEnum;
}