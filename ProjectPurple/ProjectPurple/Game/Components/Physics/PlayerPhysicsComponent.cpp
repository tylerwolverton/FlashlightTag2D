#include "PlayerPhysicsComponent.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "PortalLogicComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(ComponentId componentId,
                                               std::shared_ptr<TransformComponent> transformCompPtr, 
                                               float maxSpeed,
                                               float mass,
                                               float restitution,
                                               Vector2D<float> velocity, 
                                               Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformCompPtr, maxSpeed, mass, restitution, velocity, acceleration)
{
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

bool PlayerPhysicsComponent::SignalCollision(ActorId actorId)
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
        if (gameStateCompPtr->GetType() == "Portal")
        {
            auto logicCompPtr = actorPtr->GetLogicCompPtr();
            if (logicCompPtr != nullptr)
            {
                std::dynamic_pointer_cast<PortalLogicComponent>(logicCompPtr)->ChangeLevel();
                stopResolvingCollisions = true;
            }
        }
    }

    return stopResolvingCollisions;
}