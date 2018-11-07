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
    auto prevDir = actor.GetTransformComponent()->GetDirection();
    MoveActor(deltaMs);

    if (prevDir != actor.GetTransformComponent()->GetDirection())
    {
        actor.GetLifeComponent()->Die();
    }
}

bool ProjectilePhysicsComponent::SignalCollision(ActorId actorId)
{
    bool stopResolvingCollisions = false;

    std::shared_ptr<GameActor> actor = ServiceLocator::GetActorFactory()->GetActor(actorId);
    if (actor == nullptr)
    {
        return stopResolvingCollisions;
    }
    auto actorLifeComponent = actor->GetLifeComponent();
    if (actorLifeComponent != nullptr)
    {
        actorLifeComponent->TakeDamage(1);
    }

    std::shared_ptr<GameActor> thisActor = ServiceLocator::GetActorFactory()->GetActor(GetParentActorId());
    if (thisActor == nullptr)
    {
        return stopResolvingCollisions;
    }
    auto thisLifeComponent = thisActor->GetLifeComponent();
    if (thisLifeComponent != nullptr)
    {
        thisLifeComponent->Die();
    }

    return stopResolvingCollisions;
}