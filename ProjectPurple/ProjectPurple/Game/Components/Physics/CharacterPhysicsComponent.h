#pragma once
#include "PhysicsComponent.h"
#include "Vector2D.h"

class TransformComponent;

class CharacterPhysicsComponent :
    public PhysicsComponent
{
public:
    CharacterPhysicsComponent(ComponentId componentId,
                              const std::shared_ptr<TransformComponent>& transformCompPtr, 
                              float maxSpeed, 
                              float mass, 
                              float restitution,
                              const Vector2D<float>& velocity = Vector2D<float>(0, 0), 
                              const Vector2D<float>& acceleration = Vector2D<float>(0, 0));
    virtual ~CharacterPhysicsComponent();

    virtual bool SignalCollision(ActorId actorId) override;
};

