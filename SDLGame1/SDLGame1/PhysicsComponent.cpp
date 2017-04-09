#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
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

void PhysicsComponent::Update(GameActor& actor, int deltaMs)
{
}