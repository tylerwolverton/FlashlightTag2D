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
    //const std::shared_ptr<GameActor> GetOwner() const { return m_pOwner; }
    const ComponentId GetComponentId() const { return m_componentId; }
	virtual const EComponentNames GetComponentName() const = 0;

    void SetParentActorId(ActorId parentActorId) { m_parentActorId = parentActorId; }
    //void SetOwner(std::shared_ptr<GameActor> pOwner) { m_pOwner = pOwner; }

protected:
	//std::shared_ptr<GameActor> m_pOwner;
    ActorId m_parentActorId;

private:
    ComponentId m_componentId;
};

