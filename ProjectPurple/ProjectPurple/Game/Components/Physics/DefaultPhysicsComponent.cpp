#include "DefaultPhysicsComponent.h"

DefaultPhysicsComponent::DefaultPhysicsComponent(ComponentId componentId,
                                                 const std::shared_ptr<TransformComponent>& transformCompPtr,
                                                 float maxSpeed,
                                                 float mass,
                                                 float restitution,
                                                 const Vector2D<float>& velocity,
                                                 const Vector2D<float>& acceleration)
    : PhysicsComponent(componentId, transformCompPtr, maxSpeed, mass, restitution, velocity, acceleration)
{
}

DefaultPhysicsComponent::~DefaultPhysicsComponent()
{
}
