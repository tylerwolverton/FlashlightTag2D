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
	//m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y - 10)); 
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, -2));
}

void BaseLogicComponent::MoveDown() 
{ 
	//m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y + 10)); 
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, 2));
}

void BaseLogicComponent::MoveRight() 
{ 
	//m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x + 10, m_TransformComponent->GetPosition().y)); 
	m_pPhysicsComponent->AddForce(Vector2D<float>(2, 0));
}

void BaseLogicComponent::MoveLeft() 
{ 
	//m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x - 10, m_TransformComponent->GetPosition().y)); 
	m_pPhysicsComponent->AddForce(Vector2D<float>(-2, 0));
}