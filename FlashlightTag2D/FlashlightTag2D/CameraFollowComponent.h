#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class GameActor;

class CameraFollowComponent : public ActorComponent
{
public:
	CameraFollowComponent(ComponentId componentId);
	CameraFollowComponent(ComponentId componentId, StrongGameActorPtr target, Vector2D<int> levelSize);
	virtual ~CameraFollowComponent();

	void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;

	void SetTargetActor(StrongGameActorPtr actor);

private:
	StrongGameActorPtr m_pTarget;
    Vector2D<int> m_levelSize;
};

