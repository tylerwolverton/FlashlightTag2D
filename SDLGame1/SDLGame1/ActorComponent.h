#pragma once
#include <tinyxml2.h>

#include "Types.h"
class GameActor;

class ActorComponent
{
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual bool Init(tinyxml2::XMLElement* pData) = 0;
	virtual void PostInit() {}
	virtual void Update(GameActor& actor, int deltaMs) {}

	virtual ComponentId GetComponentId() const = 0;
	virtual EComponentNames GetComponentName() const = 0;

	static ComponentId GetIdFromName(const char* componentStr)
	{
		//void* rawId = HashedString::hash_name(componentStr);
		return ComponentIdNameMap.find(componentStr)->second;
	}

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

