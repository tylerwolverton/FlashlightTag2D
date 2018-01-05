#pragma once
#include <tinyxml2.h>

#include "Types.h"
class GameActor;

class ActorComponent
{
	friend class ActorFactory;

public:
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual void Update(GameActor& actor, float deltaMs) {}

	virtual ComponentId GetComponentId() const = 0;
	virtual EComponentNames GetComponentName() const = 0;

	static ComponentId GetIdFromName(const char* componentStr)
	{
		return ComponentIdNameMap.find(componentStr)->second;
	}

protected:
	StrongActorPtr m_pOwner;

private:
	void set_owner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

