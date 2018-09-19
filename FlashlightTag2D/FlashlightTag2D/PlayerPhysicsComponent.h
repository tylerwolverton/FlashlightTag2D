#pragma once
#include "PhysicsComponent.h"
#include "Vector2D.h"

class TransformComponent;

class PlayerPhysicsComponent :
	public PhysicsComponent
{
public:
	PlayerPhysicsComponent(ComponentId componentId, 
                           std::shared_ptr<TransformComponent> transformComponent,
		                   float maxSpeed,
		                   float mass,
                           float restitution,
		                   Vector2D<float> velocity = Vector2D<float>(0, 0),
		                   Vector2D<float> acceleration = Vector2D<float>(0, 0));
	virtual ~PlayerPhysicsComponent();

	virtual void SignalCollision(ActorId actorId) override;
};

