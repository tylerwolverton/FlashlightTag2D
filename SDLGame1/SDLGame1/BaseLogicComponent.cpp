#include "BaseLogicComponent.h"

BaseLogicComponent::BaseLogicComponent(std::shared_ptr<TransformComponent> transformComponent)
	: m_TransformComponent(transformComponent)
{
}

BaseLogicComponent::~BaseLogicComponent()
{
}

bool BaseLogicComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
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
	m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y - 10)); 
}

void BaseLogicComponent::MoveDown() 
{ 
	m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y + 10)); 
}

void BaseLogicComponent::MoveRight() 
{ 
	m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x + 10, m_TransformComponent->GetPosition().y)); 
}

void BaseLogicComponent::MoveLeft() 
{ 
	m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x - 10, m_TransformComponent->GetPosition().y)); 
}