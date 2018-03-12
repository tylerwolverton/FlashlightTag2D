#pragma once
#include "Types.h"

class GameActor;

class ActorComponent
{
	friend class ActorFactory;

public:
    ActorComponent(ComponentId componentId) 
        : m_componentId(componentId) {}
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual void Update(GameActor& actor, float deltaMs) {}

    const ComponentId GetComponentId() const { return m_componentId; }
	virtual EComponentNames GetComponentName() const = 0;
    
protected:
	StrongActorPtr m_pOwner;

private:
	void setOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }

    ComponentId m_componentId;
};

