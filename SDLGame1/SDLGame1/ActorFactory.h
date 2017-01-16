#pragma once
#include <memory>
#include <map>
#include <tinyxml2.h>

#include "Types.h"

class ActorFactory
{
	ActorId m_lastActorId;

protected:
	ActorComponentCreatorMap m_actorComponentCreators;
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

public:
	ActorFactory();
	
	StrongActorPtr CreateActor(const char* actorResource);

	virtual StrongActorComponentPtr VCreateComponent(tinyxml2::XMLElement* pData);

protected:
	virtual StrongActorComponentPtr ActorComponentCreator();

private:
	ActorId GetNextActorId() { ++m_lastActorId; return m_lastActorId; };
};

