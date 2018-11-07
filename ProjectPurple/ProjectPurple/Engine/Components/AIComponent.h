#pragma once
#include "ActorComponent.h"

#include <vector>

class GameActor;
class Behavior;

class AIComponent :
    public ActorComponent
{
public:
    AIComponent(ComponentId componentId, std::vector<std::shared_ptr<Behavior>> behaviorVec);
    virtual ~AIComponent();

    void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;

    //std::shared_ptr<Behavior> GetBehavior() { return m_behavior; }
    //void SetBehavior(std::shared_ptr<Behavior> behavior) { m_behavior = behavior; }

private:
    std::vector<std::shared_ptr<Behavior>> m_behaviorVec;
};

