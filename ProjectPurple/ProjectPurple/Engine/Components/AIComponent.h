#pragma once
#include "ActorComponent.h"

#include <vector>

class GameActor;
class Behavior;

class AIComponent :
    public ActorComponent
{
public:
    AIComponent(ComponentId componentId, std::vector<std::shared_ptr<Behavior>> behaviorPtrVec);
    virtual ~AIComponent();

    void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;

private:
    std::vector<std::shared_ptr<Behavior>> m_behaviorPtrVec;
};

