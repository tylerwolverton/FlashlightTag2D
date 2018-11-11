#include "PickupPhysicsComponent.h"
#include "GameActor.h"
#include "LifeComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

PickupPhysicsComponent::PickupPhysicsComponent(ComponentId componentId,
                                               std::shared_ptr<TransformComponent> transformCompPtr, 
                                               float maxSpeed,
                                               float mass,
                                               float restitution,
                                               Vector2D<float> velocity, 
                                               Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformCompPtr, maxSpeed, mass, restitution, velocity, acceleration)
{
}

PickupPhysicsComponent::~PickupPhysicsComponent()
{
}

bool PickupPhysicsComponent::SignalCollision(ActorId actorId)
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
            auto thisActorPtr = ServiceLocator::GetActorFactory()->GetActor(GetParentActorId());
            if (thisActorPtr == nullptr)
            {
                return stopResolvingCollisions;
            }

            auto thisGameStateCompPtr = thisActorPtr->GetGameStateCompPtr();
            if (thisGameStateCompPtr != nullptr)
            {
                std::dynamic_pointer_cast<PlayerGameStateComponent>(gameStateCompPtr)->AddToInventory(thisGameStateCompPtr);
            }

            auto thisLifeCompPtr = thisActorPtr->GetLifeCompPtr();
            if (thisLifeCompPtr != nullptr)
            {
                thisLifeCompPtr->Die();
            }
        }
    }

    return stopResolvingCollisions;
}