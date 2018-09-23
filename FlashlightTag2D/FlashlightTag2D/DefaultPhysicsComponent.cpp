#include "DefaultPhysicsComponent.h"

DefaultPhysicsComponent::DefaultPhysicsComponent(ComponentId componentId,
                                               std::shared_ptr<TransformComponent> transformComponent,
                                               float maxSpeed,
                                               float mass,
                                               float restitution,
                                               Vector2D<float> velocity,
                                               Vector2D<float> acceleration)
    : PhysicsComponent(componentId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
{
}

DefaultPhysicsComponent::~DefaultPhysicsComponent()
{
}
