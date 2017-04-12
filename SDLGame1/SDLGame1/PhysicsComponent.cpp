#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent)
	: m_TransformComponent(transformComponent)
{
}

PhysicsComponent::~PhysicsComponent()
{
}

ComponentId PhysicsComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames PhysicsComponent::GetComponentName() const
{
	return EComponentNames::PhysicsComponentEnum;
}