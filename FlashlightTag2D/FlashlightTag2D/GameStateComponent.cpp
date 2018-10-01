#include "GameStateComponent.h"
#include "HideBehavior.h"
//#include "SeekBehavior.h"
//#include "RushBehavior.h"
//#include "SpawnBehavior.h"

GameStateComponent::GameStateComponent(ComponentId componentId, std::string actorName)
	: ActorComponent(componentId),
	  m_actorName(actorName)
{
	//updateBehavior();
}

GameStateComponent::~GameStateComponent()
{
}

// Maybe too much for this component to do? AI could actually select the behavior and this could just set the role
// Player also needs role but not necessarily the same behavior
void GameStateComponent::Update(GameActor& actor, float deltaMs)
{
	// Check for a role change and update behavior
	/*if (m_curRole != m_prevRole)
	{
		updateBehavior();

		m_prevRole = m_curRole;
	}*/
}

//void GameStateComponent::updateBehavior()
//{
//	switch (m_curRole)
//	{
//		/*case EGameRole::Hider:
//			m_behavior = std::make_shared<HideBehavior>();
//			break;*/
//		case EGameBehavior::Seek:
//			//m_behavior = std::make_shared<SeekBehavior>(m_levelSize);
//			break;
//        case EGameBehavior::Rush:
//            m_behavior = std::make_shared<RushBehavior>();
//            break;
//        case EGameBehavior::Spawn:
//            //m_behavior = std::make_shared<SpawnBehavior>();
//            break;
//		default:
//			break;
//	}
//}

const EComponentNames GameStateComponent::GetComponentName() const
{
	return EComponentNames::GameStateComponentEnum;
}