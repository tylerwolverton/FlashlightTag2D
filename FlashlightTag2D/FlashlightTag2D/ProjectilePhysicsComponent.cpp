#include "ProjectilePhysicsComponent.h"
#include "TransformComponent.h"
#include "LifeComponent.h"
#include "GameActor.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

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

void ProjectilePhysicsComponent::SignalCollision(ActorId actorId)
{
    std::shared_ptr<GameActor> actor = ServiceLocator::GetActorFactory()->GetActor(actorId);
    if (actor == nullptr)
    {
        return;
    }
    auto actorLifeComponent = actor->GetLifeComponent();
    if (actorLifeComponent != nullptr)
    {
        actorLifeComponent->TakeDamage(1);
    }

    std::shared_ptr<GameActor> thisActor = ServiceLocator::GetActorFactory()->GetActor(GetParentActorId());
    if (thisActor == nullptr)
    {
        return;
    }
    auto thisLifeComponent = thisActor->GetLifeComponent();
    if (thisLifeComponent != nullptr)
    {
        thisLifeComponent->Die();
    }
}