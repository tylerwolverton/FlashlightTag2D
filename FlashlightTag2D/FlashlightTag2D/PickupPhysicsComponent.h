#pragma once
#include "PhysicsComponent.h"

class PickupPhysicsComponent :
    public PhysicsComponent
{
public:
    PickupPhysicsComponent(ComponentId componentId, 
                           std::shared_ptr<TransformComponent> transformComponent,
		                   float maxSpeed,
		                   float mass,
                           float restitution,
		                   Vector2D<float> velocity = Vector2D<float>(0, 0),
		                   Vector2D<float> acceleration = Vector2D<float>(0, 0));

    virtual ~PickupPhysicsComponent();

    bool SignalCollision(ActorId actorId) override;
};

