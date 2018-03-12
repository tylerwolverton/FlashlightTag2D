#pragma once
#include "ActorComponent.h"

class GameActor;

class AIComponent :
	public ActorComponent
{
public:
	AIComponent(ComponentId componentId);
	virtual ~AIComponent();

	void Update(GameActor& actor, float deltaMs) override;

	virtual EComponentNames GetComponentName() const override;
};

