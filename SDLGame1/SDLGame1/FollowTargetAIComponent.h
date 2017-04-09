#pragma once
#include "ActorComponent.h"

class GameActor;

class FollowTargetAIComponent : public ActorComponent
{
public:
	FollowTargetAIComponent();
	FollowTargetAIComponent(StrongGameActorPtr p_target);
	virtual ~FollowTargetAIComponent();

	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;
	virtual EComponentNames GetComponentName() const override;

	void SetTargetActor(StrongGameActorPtr actor);

private:
	StrongGameActorPtr target;
};

