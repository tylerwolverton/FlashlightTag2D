#pragma once
#include "ActorComponent.h"

class GameActor;

class FollowTargetAIComponent : public ActorComponent
{
public:
	FollowTargetAIComponent();
	FollowTargetAIComponent(StrongGameActorPtr p_target);
	virtual ~FollowTargetAIComponent();

	bool Init(tinyxml2::XMLElement* pData) override;
	void PostInit() override;
	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;

	void SetTargetActor(StrongGameActorPtr actor);

private:
	StrongGameActorPtr target;
};

