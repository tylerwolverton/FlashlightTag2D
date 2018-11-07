#include "PickupPhysicsComponent.h"
#include "GameActor.h"
#include "LifeComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

PickupPhysicsComponent::PickupPhysicsComponent(ComponentId componentId,
                                               std::shared_ptr<TransformComponent> transformComponent, 
                                               float maxSpeed,
                                               float mass,
                                               float restitution,
                                               Vector2D<float> velocity, 
                                               Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
{
}

PickupPhysicsComponent::~PickupPhysicsComponent()
{
}

bool PickupPhysicsComponent::SignalCollision(ActorId actorId)
{
    bool stopResolvingCollisions = false;

    std::shared_ptr<GameActor> actor = ServiceLocator::GetActorFactory()->GetActor(actorId);
    if (actor == nullptr)
    {
        return stopResolvingCollisions;
    }

    auto gameStateComponent = actor->GetGameStateComponent();
    if (gameStateComponent != nullptr)
    {
        if (gameStateComponent->GetName() == "Player")
        {
            std::shared_ptr<GameActor> thisActor = ServiceLocator::GetActorFactory()->GetActor(GetParentActorId());
            if (thisActor == nullptr)
            {
                return stopResolvingCollisions;
            }

            auto thisGameStateComponent = thisActor->GetGameStateComponent();
            if (thisGameStateComponent != nullptr)
            {
                std::dynamic_pointer_cast<PlayerGameStateComponent>(gameStateComponent)->AddToInventory(thisGameStateComponent);
            }

            auto thisLifeComponent = thisActor->GetLifeComponent();
            if (thisLifeComponent != nullptr)
            {
                thisLifeComponent->Die();
            }
        }
    }

    return stopResolvingCollisions;
}