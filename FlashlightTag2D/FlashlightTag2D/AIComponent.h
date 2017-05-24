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

	CommandList SimpleMove();

	ComponentId GetComponentId() const;
	virtual EComponentNames GetComponentName() const override;

private:
	int moveTimer;
	int moveDirection;
	int moveLength;
};

