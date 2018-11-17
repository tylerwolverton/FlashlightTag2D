#pragma once
#include "PhysicsComponent.h"
#include "Vector2D.h"

class TransformComponent;

class PlayerPhysicsComponent :
    public PhysicsComponent
{
public:
    PlayerPhysicsComponent(ComponentId componentId,
                           const std::shared_ptr<TransformComponent>& transformCompPtr, 
                           float maxSpeed, 
                           float mass, 
                           float restitution,
                           const Vector2D<float>& velocity = Vector2D<float>(0, 0), 
                           const Vector2D<float>& acceleration = Vector2D<float>(0, 0));
    virtual ~PlayerPhysicsComponent();

    virtual bool SignalCollision(ActorId actorId) override;
};

