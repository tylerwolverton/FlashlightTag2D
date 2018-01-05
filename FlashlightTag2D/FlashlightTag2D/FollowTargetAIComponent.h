#pragma once
#include "ActorComponent.h"

class GameActor;

class FollowTargetAIComponent : public ActorComponent
{
public:
	FollowTargetAIComponent();
	FollowTargetAIComponent(StrongGameActorPtr target);
	virtual ~FollowTargetAIComponent();

	void Update(GameActor& actor, float deltaMs) override;

	ComponentId GetComponentId() const;
	virtual EComponentNames GetComponentName() const override;

	void SetTargetActor(StrongGameActorPtr actor);

private:
	StrongGameActorPtr m_pTarget;
};

