#pragma once
#include "PhysicsComponent.h"

class TransformComponent;

class PlayerPhysicsComponent :
	public PhysicsComponent
{
public:
	PlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent,
		float maxSpeed,
		float mass,
		Vector2D<float> velocity = Vector2D<float>(0, 0),
		Vector2D<float> acceleration = Vector2D<float>(0, 0));
	virtual ~PlayerPhysicsComponent();

	virtual void SignalCollision(GameActor& actor) override;
};

