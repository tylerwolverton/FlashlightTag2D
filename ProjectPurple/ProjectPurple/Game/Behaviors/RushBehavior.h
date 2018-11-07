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
    std::vector<std::shared_ptr<Command>> moveTowardsTarget(std::shared_ptr<TransformComponent> thisActorTransformComponent,
                                                            std::shared_ptr<TransformComponent> targetActorTransformComponent,
                                                            float speed);
    
    std::shared_ptr<GameActor> m_targetActor;
    bool m_chaseActor;

};

