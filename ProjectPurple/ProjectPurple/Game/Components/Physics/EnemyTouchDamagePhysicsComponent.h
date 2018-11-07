#pragma once
#include "CharacterPhysicsComponent.h"

class EnemyTouchDamagePhysicsComponent :
    public CharacterPhysicsComponent
{
public:
    EnemyTouchDamagePhysicsComponent(ComponentId componentId, 
                                     std::shared_ptr<TransformComponent> transformComponent,
                                     float maxSpeed,
                                     float mass,
                                     float restitution,
                                     Vector2D<float> velocity = Vector2D<float>(0, 0),
                                     Vector2D<float> acceleration = Vector2D<float>(0, 0));
    virtual ~EnemyTouchDamagePhysicsComponent();

    virtual bool SignalCollision(ActorId actorId) override;
};

