#pragma once
#include "PhysicsComponent.h"
class ProjectilePhysicsComponent :
    public PhysicsComponent
{
public:
    ProjectilePhysicsComponent(ComponentId componentId,
                               const std::shared_ptr<TransformComponent>& transformCompPtr, 
                               float maxSpeed, 
                               float mass, 
                               float restitution,
                               const Vector2D<float>& velocity = Vector2D<float>(0, 0), 
                               const Vector2D<float>& acceleration = Vector2D<float>(0, 0));
    virtual ~ProjectilePhysicsComponent();

    void virtual Update(GameActor& actor, float deltaMs) override;

    virtual bool SignalCollision(ActorId actorId) override;
};

