#pragma once
#include "ActorComponent.h"
#include "Behavior.h"

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

	void SetBehavior(Behavior* behavior) { curBehavior = behavior; }

private:
	int m_moveTimer;
	int m_moveDirection;
	int m_moveLength;
	Behavior* curBehavior;
};

