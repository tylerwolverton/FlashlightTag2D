#include "PlayerPhysicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent)
	: PhysicsComponent(transformComponent)
{
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

void PlayerPhysicsComponent::SignalCollision(GameActor& actor)
{
	std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actor.GetComponentByName(TransformComponentEnum));
	Vector2D<float> dist = rawActorTransformComponent->GetPosition() - m_TransformComponent->GetPosition();
	m_TransformComponent->SetPosition(m_TransformComponent->GetPosition() + (dist.Normalize() * -5));
}