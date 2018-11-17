#pragma once
#include "CharacterPhysicsComponent.h"

class EnemyTouchDamagePhysicsComponent :
    public CharacterPhysicsComponent
{
public:
    EnemyTouchDamagePhysicsComponent(ComponentId componentId,
                                     const std::shared_ptr<TransformComponent>& transformCompPtr, 
                                     float maxSpeed, 
                                     float mass, 
                                     float restitution,
                                     const Vector2D<float>& velocity = Vector2D<float>(0, 0), 
                                     const Vector2D<float>& acceleration = Vector2D<float>(0, 0));
    virtual ~EnemyTouchDamagePhysicsComponent();

    virtual bool SignalCollision(ActorId actorId) override;
};

