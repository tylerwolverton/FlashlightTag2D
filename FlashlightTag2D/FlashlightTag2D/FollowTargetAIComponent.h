#pragma once
#include "ActorComponent.h"

class GameActor;

class FollowTargetAIComponent : public ActorComponent
{
public:
	FollowTargetAIComponent(ComponentId componentId);
	FollowTargetAIComponent(ComponentId componentId, StrongGameActorPtr target);
	virtual ~FollowTargetAIComponent();

	void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;

	void SetTargetActor(StrongGameActorPtr actor);

private:
	StrongGameActorPtr m_pTarget;
};

