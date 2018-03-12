#include "BaseLogicComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

BaseLogicComponent::BaseLogicComponent(ComponentId componentId, std::shared_ptr<TransformComponent> transformComponent)
	: ActorComponent(componentId),
      m_pTransformComponent(transformComponent)
{
}

BaseLogicComponent::BaseLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent)
	: ActorComponent(componentId),
      m_pPhysicsComponent(physicsComponent)
{
}

BaseLogicComponent::~BaseLogicComponent()
{
}

EComponentNames BaseLogicComponent::GetComponentName() const
{
	return EComponentNames::BaseLogicComponentEnum;
}

void BaseLogicComponent::MoveUp()
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, 2));
}

void BaseLogicComponent::MoveDown() 
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, -2));
}

void BaseLogicComponent::MoveRight() 
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(2, 0));
}

void BaseLogicComponent::MoveLeft() 
{ 
	m_pPhysicsComponent->AddForce(Vector2D<float>(-2, 0));
}