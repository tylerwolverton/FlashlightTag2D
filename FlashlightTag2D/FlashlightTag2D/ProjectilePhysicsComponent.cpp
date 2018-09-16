#include "ProjectilePhysicsComponent.h"
#include "TransformComponent.h"
#include "LifeComponent.h"
#include "GameActor.h"

ProjectilePhysicsComponent::ProjectilePhysicsComponent(ComponentId componentId,
                                                       std::shared_ptr<TransformComponent> transformComponent,
                                                       float maxSpeed,
                                                       float mass,
                                                       float restitution,
                                                       Vector2D<float> velocity,
                                                       Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
{
}

ProjectilePhysicsComponent::~ProjectilePhysicsComponent()
{
}

void ProjectilePhysicsComponent::Update(GameActor& actor, float deltaMs)
{
	MoveActor(deltaMs);
}

void ProjectilePhysicsComponent::SignalCollision(GameActor& actor)
{
    auto actorLifeComponent = actor.GetLifeComponent();
    if (actorLifeComponent != nullptr)
    {
        actorLifeComponent->TakeDamage(1);
    }

    auto thisLifeComponent = this->m_pOwner->GetLifeComponent();
    if (thisLifeComponent != nullptr)
    {
        thisLifeComponent->Die();
    }
}