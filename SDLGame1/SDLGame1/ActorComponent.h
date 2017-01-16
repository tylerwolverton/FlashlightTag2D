#pragma once
#include <tinyxml2.h>

#include "Types.h"

class ActorComponent
{
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual bool VInit(tinyxml2::XMLElement* pData) = 0;
	virtual void VPostInit() {}
	virtual void VUpdate(int deltaMs) {}

	virtual ComponentId VGetComponentId() const = 0;

	static ComponentId GetIdFromName(const char* componentStr)
	{
		//void* rawId = HashedString::hash_name(componentStr);
		return ComponentIdNameMap.find(componentStr)->second;
	}

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

