#pragma once
#include "ActorComponent.h"

class GameActor;

class FollowTargetAIComponent : public ActorComponent
{
public:
	FollowTargetAIComponent();
	FollowTargetAIComponent(const GameActor& p_target);
	virtual ~FollowTargetAIComponent();

	bool Init(tinyxml2::XMLElement* pData) override;
	void PostInit() override;
	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;

	void SetTargetActor(const GameActor& actor);

private:
	StrongGameActorPtr target;
};

