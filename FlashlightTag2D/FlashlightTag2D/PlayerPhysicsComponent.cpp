#include "PlayerPhysicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent, 
                                               float maxSpeed,
                                               float mass,
                                               float restitution,
                                               Vector2D<float> velocity, 
                                               Vector2D<float> acceleration)
	: PhysicsComponent(transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
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

	//Vector2D<float> dist = actorTransformComponent->GetPosition() - m_pTransformComponent->GetPosition();
	//m_pTransformComponent->SetPosition(m_pTransformComponent->GetPosition() + (dist.Normalize() * -5));
}