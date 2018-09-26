#include "GameStateComponent.h"
#include "HideBehavior.h"
#include "SeekBehavior.h"
#include "RushBehavior.h"

GameStateComponent::GameStateComponent(ComponentId componentId, std::string actorName, EGameRole role)
	: ActorComponent(componentId),
      m_curRole(role),
	  m_prevRole(role),
	  m_actorName(actorName)
{
	updateBehavior();
}

GameStateComponent::~GameStateComponent()
{
}

// Maybe too much for this component to do? AI could actually select the behavior and this could just set the role
// Player also needs role but not necessarily the same behavior
void GameStateComponent::Update(GameActor& actor, float deltaMs)
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
		/*case EGameRole::Hider:
			m_behavior = std::make_shared<HideBehavior>();
			break;*/
		case EGameRole::Seeker:
			//m_behavior = std::make_shared<SeekBehavior>(m_levelSize);
			break;
        case EGameRole::Rusher:
            m_behavior = std::make_shared<RushBehavior>();
            break;
		case EGameRole::Out:
			break;
		default:
			break;
	}
}

const EComponentNames GameStateComponent::GetComponentName() const
{
	return EComponentNames::GameStateComponentEnum;
}