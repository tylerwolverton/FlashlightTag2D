#include "DefaultPhysicsComponent.h"

DefaultPhysicsComponent::DefaultPhysicsComponent(ComponentId componentId,
                                                 std::shared_ptr<TransformComponent> transformCompPtr,
                                                 float maxSpeed,
                                                 float mass,
                                                 float restitution,
                                                 Vector2D<float> velocity,
                                                 Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformCompPtr, maxSpeed, mass, restitution, velocity, acceleration)
{
}

DefaultPhysicsComponent::~DefaultPhysicsComponent()
{
}
