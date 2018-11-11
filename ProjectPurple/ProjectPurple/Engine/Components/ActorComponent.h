#pragma once
#include "Types.h"

#include <memory>

class GameActor;

class ActorComponent
{
public:
    ActorComponent(ComponentId componentId) 
        : m_componentId(componentId) {}
    virtual ~ActorComponent() {}

    virtual void Update(GameActor& actor, float deltaMs) {}

    const ActorId GetParentActorId() const { return m_parentActorId; }
    void SetParentActorId(ActorId parentActorId) { m_parentActorId = parentActorId; }
    const std::shared_ptr<GameActor> GetParent() const;
    const ComponentId GetComponentId() const { return m_componentId; }
    virtual const EComponentNames GetComponentName() const = 0;

protected:
    ActorId m_parentActorId;

private:
    ComponentId m_componentId;
};

