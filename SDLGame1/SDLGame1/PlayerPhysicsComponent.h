#pragma once
#include "PhysicsComponent.h"

class TransformComponent;

class PlayerPhysicsComponent :
	public PhysicsComponent
{
public:
	PlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent);
	virtual ~PlayerPhysicsComponent();

	virtual void SignalCollision(GameActor& actor) override;

};

