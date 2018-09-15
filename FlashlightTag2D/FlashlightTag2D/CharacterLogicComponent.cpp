#include "CharacterLogicComponent.h"
#include "ActorFactory.h"
#include "PhysicsComponent.h"
#include "ServiceLocator.h"

CharacterLogicComponent::CharacterLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent)
	: LogicComponent(componentId),
	  m_pPhysicsComponent(physicsComponent)
{
}

CharacterLogicComponent::~CharacterLogicComponent()
{
}

void CharacterLogicComponent::MoveUp()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, m_pPhysicsComponent->GetCurSpeed()));
}

void CharacterLogicComponent::MoveDown()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, -m_pPhysicsComponent->GetCurSpeed()));
}

void CharacterLogicComponent::MoveRight()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(m_pPhysicsComponent->GetCurSpeed(), 0));
}

void CharacterLogicComponent::MoveLeft()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(-m_pPhysicsComponent->GetCurSpeed(), 0));
}

void CharacterLogicComponent::Shoot()
{
	auto actorFactory = ServiceLocator::GetActorFactory();

}