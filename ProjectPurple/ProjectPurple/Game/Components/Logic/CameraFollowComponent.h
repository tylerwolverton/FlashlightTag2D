#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class GameActor;

class CameraFollowComponent : public ActorComponent
{
public:
    CameraFollowComponent(ComponentId componentId);
    CameraFollowComponent(ComponentId componentId, std::shared_ptr<GameActor> target, Vector2D<int> levelSize);
    virtual ~CameraFollowComponent();

    void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;

    void SetTargetActor(std::shared_ptr<GameActor> actor);

private:
    std::shared_ptr<GameActor> m_targetActorPtr;
    Vector2D<int> m_levelSize;
};

