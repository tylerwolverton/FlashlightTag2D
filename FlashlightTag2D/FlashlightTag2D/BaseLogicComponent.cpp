#include "BaseLogicComponent.h"

BaseLogicComponent::BaseLogicComponent(std::shared_ptr<TransformComponent> transformComponent)
	: m_pTransformComponent(transformComponent)
{
}

BaseLogicComponent::BaseLogicComponent(std::shared_ptr<PhysicsComponent> physicsComponent)
	: m_pPhysicsComponent(physicsComponent)
{
}

BaseLogicComponent::~BaseLogicComponent()
{
}

ComponentId BaseLogicComponent::GetComponentId() const
{
	return ComponentId();
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