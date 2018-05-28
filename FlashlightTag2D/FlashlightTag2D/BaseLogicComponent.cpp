#include "BaseLogicComponent.h"
#include "PhysicsComponent.h"

BaseLogicComponent::BaseLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent)
	: ActorComponent(componentId),
      m_pPhysicsComponent(physicsComponent)
{
}

BaseLogicComponent::BaseLogicComponent(int actorId, ComponentId componentId)
    : ActorComponent(componentId),
      m_actorId(actorId)
{
}

BaseLogicComponent::~BaseLogicComponent()
{
}

const EComponentNames BaseLogicComponent::GetComponentName() const
{
	return EComponentNames::BaseLogicComponentEnum;
}

void BaseLogicComponent::MoveUp()
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, m_pPhysicsComponent->GetCurSpeed()));
}

void BaseLogicComponent::MoveDown() 
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, -m_pPhysicsComponent->GetCurSpeed()));
}

void BaseLogicComponent::MoveRight() 
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(m_pPhysicsComponent->GetCurSpeed(), 0));
}

void BaseLogicComponent::MoveLeft() 
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(-m_pPhysicsComponent->GetCurSpeed(), 0));
}