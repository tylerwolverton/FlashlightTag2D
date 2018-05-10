#pragma once
#include "Types.h"

#include <memory>

class Actor;
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
	virtual const EComponentNames GetComponentName() const = 0;
    
protected:
	std::shared_ptr<Actor> m_pOwner;

private:
	void setOwner(std::shared_ptr<Actor> pOwner) { m_pOwner = pOwner; }

    ComponentId m_componentId;
};

