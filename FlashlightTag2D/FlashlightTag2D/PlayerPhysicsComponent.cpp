#include "PlayerPhysicsComponent.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "PortalLogicComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

PlayerPhysicsComponent::PlayerPhysicsComponent(ComponentId componentId,
                                               std::shared_ptr<TransformComponent> transformComponent, 
                                               float maxSpeed,
                                               float mass,
                                               float restitution,
                                               Vector2D<float> velocity, 
                                               Vector2D<float> acceleration)
	: PhysicsComponent(componentId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
{
}

PlayerPhysicsComponent::~PlayerPhysicsComponent()
{
}

bool PlayerPhysicsComponent::SignalCollision(ActorId actorId)
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
        if (gameStateComponent->GetName() == "Portal")
        {
            auto logicComponent = actor->GetLogicComponent();
            if (logicComponent != nullptr)
            {
                std::dynamic_pointer_cast<PortalLogicComponent>(logicComponent)->ChangeLevel();
                stopResolvingCollisions = true;
            }
        }
    }

    return stopResolvingCollisions;
}