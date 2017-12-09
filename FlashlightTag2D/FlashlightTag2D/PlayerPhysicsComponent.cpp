#include "PlayerPhysicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent, Vector2D<float> velocity, float maxSpeed, float mass)
	: PhysicsComponent(transformComponent, velocity, maxSpeed, mass)
{
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

void PlayerPhysicsComponent::SignalCollision(GameActor& actor)
{
	auto actorTransformComponent = actor.GetTransformComponent();
    if (actorTransformComponent == nullptr)
    {
        return;
    }

	Vector2D<float> dist = actorTransformComponent->GetPosition() - m_pTransformComponent->GetPosition();
	m_pTransformComponent->SetPosition(m_pTransformComponent->GetPosition() + (dist.Normalize() * -5));
}