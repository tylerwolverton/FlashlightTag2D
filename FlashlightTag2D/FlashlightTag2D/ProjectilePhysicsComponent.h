#pragma once
#include "PhysicsComponent.h"
class ProjectilePhysicsComponent :
	public PhysicsComponent
{
public:
	ProjectilePhysicsComponent(ComponentId componentId,
							   std::shared_ptr<TransformComponent> transformComponent,
							   float maxSpeed,
							   float mass,
							   float restitution,
							   Vector2D<float> velocity = Vector2D<float>(0, 0),
							   Vector2D<float> acceleration = Vector2D<float>(0, 0));
	virtual ~ProjectilePhysicsComponent();

	virtual void SignalCollision(GameActor& actor) override;
};

