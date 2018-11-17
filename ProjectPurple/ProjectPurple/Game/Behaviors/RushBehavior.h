#pragma once
#include "Behavior.h"
#include "Vector2D.h"

class RushBehavior :
    public Behavior
{
public:
    RushBehavior();
    virtual ~RushBehavior();

    virtual std::vector<std::shared_ptr<Command>> Update(const GameActor& thisActor) override;

private:
	std::shared_ptr<GameActor> m_targetActorPtr;
	bool m_chaseActor;

    std::vector<std::shared_ptr<Command>> moveTowardsTarget(const std::shared_ptr<TransformComponent>& thisActorTransformCompPtr,
                                                            const std::shared_ptr<TransformComponent>& targetActorTransformCompPtr,
                                                            float speed);
};

