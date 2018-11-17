#include "EnemyTouchDamagePhysicsComponent.h"
#include "GameActor.h"
#include "LifeComponent.h"
#include "GameStateComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

EnemyTouchDamagePhysicsComponent::EnemyTouchDamagePhysicsComponent(ComponentId componentId,
                                                                   const std::shared_ptr<TransformComponent>& transformCompPtr, 
                                                                   float maxSpeed,
                                                                   float mass,
                                                                   float restitution,
                                                                   const Vector2D<float>& velocity, 
                                                                   const Vector2D<float>& acceleration)
    : CharacterPhysicsComponent(componentId, transformCompPtr, maxSpeed, mass, restitution, velocity, acceleration)
{
}

EnemyTouchDamagePhysicsComponent::~EnemyTouchDamagePhysicsComponent()
{
}

bool EnemyTouchDamagePhysicsComponent::SignalCollision(ActorId actorId)
{
    bool stopResolvingCollisions = false;

    auto actorPtr = ServiceLocator::GetActorFactory()->GetActor(actorId);
    if (actorPtr == nullptr)
    {
        return stopResolvingCollisions;
    }

    auto gameStateCompPtr = actorPtr->GetGameStateCompPtr();
    if (gameStateCompPtr != nullptr)
    {
        if (gameStateCompPtr->GetName() == "Player")
        {
            auto lifeCompPtr = actorPtr->GetLifeCompPtr();
            if (lifeCompPtr != nullptr)
            {
                lifeCompPtr->TakeDamage(1);
            }
        }
    }

    return stopResolvingCollisions;
}