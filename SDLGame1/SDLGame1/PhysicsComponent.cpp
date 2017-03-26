#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::~PhysicsComponent()
{
}

bool PhysicsComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
}

void PhysicsComponent::PostInit()
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