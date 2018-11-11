#include "ProjectilePhysicsComponent.h"
#include "TransformComponent.h"
#include "LifeComponent.h"
#include "GameActor.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

ProjectilePhysicsComponent::ProjectilePhysicsComponent(ComponentId componentId,
                                                       std::shared_ptr<TransformComponent> transformCompPtr,
                                                       float maxSpeed,
                                                       float mass,
                                                       float restitution,
                                                       Vector2D<float> velocity,
                                                       Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformCompPtr, maxSpeed, mass, restitution, velocity, acceleration)
{
}

ProjectilePhysicsComponent::~ProjectilePhysicsComponent()
{
}

void ProjectilePhysicsComponent::Update(GameActor& actor, float deltaMs)
{
    Vector2D<float> prevDir = actor.GetTransformCompPtr()->GetDirection();
    MoveActor(deltaMs);

    if (prevDir != actor.GetTransformCompPtr()->GetDirection())
    {
        actor.GetLifeCompPtr()->Die();
    }
}

bool ProjectilePhysicsComponent::SignalCollision(ActorId actorId)
{
    bool stopResolvingCollisions = false;

    auto actorPtr = ServiceLocator::GetActorFactory()->GetActor(actorId);
    if (actorPtr == nullptr)
    {
        return stopResolvingCollisions;
    }
    auto actorLifeCompPtr = actorPtr->GetLifeCompPtr();
    if (actorLifeCompPtr != nullptr)
    {
        actorLifeCompPtr->TakeDamage(1);
    }

    auto thisActorPtr = ServiceLocator::GetActorFactory()->GetActor(GetParentActorId());
    if (thisActorPtr == nullptr)
    {
        return stopResolvingCollisions;
    }
    auto thisLifeCompPtr = thisActorPtr->GetLifeCompPtr();
    if (thisLifeCompPtr != nullptr)
    {
        thisLifeCompPtr->Die();
    }

    return stopResolvingCollisions;
}