#pragma once
#include "PhysicsComponent.h"

class DefaultPhysicsComponent :
    public PhysicsComponent
{
public:
    DefaultPhysicsComponent(ComponentId componentId,
                            const std::shared_ptr<TransformComponent>& transformCompPtr, 
                            float maxSpeed, 
                            float mass, 
                            float restitution,
                            const Vector2D<float>& velocity = Vector2D<float>(0, 0), 
                            const Vector2D<float>& acceleration = Vector2D<float>(0, 0));
    virtual ~DefaultPhysicsComponent();
    
    virtual bool SignalCollision(ActorId actorId) override { return false; };
};

