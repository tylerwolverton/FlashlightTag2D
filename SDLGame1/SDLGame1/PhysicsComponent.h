#pragma once
#include "ActorComponent.h"
class PhysicsComponent :
	public ActorComponent
{
public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	virtual void Update(GameActor& actor, int deltaMs);

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;
};

