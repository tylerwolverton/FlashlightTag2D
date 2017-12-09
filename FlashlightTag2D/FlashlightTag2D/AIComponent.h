#pragma once
#include "ActorComponent.h"

class GameActor;

class AIComponent :
	public ActorComponent
{
public:
	AIComponent();
	virtual ~AIComponent();

	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;
	virtual EComponentNames GetComponentName() const override;
};

