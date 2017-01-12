#pragma once
#include <memory>
#include <map>

#include "Types.h"

class ActorFactory
{
	ActorId m_lastActorId;

protected:
	ActorComponentCreatorMap m_actorComponentCreators;

public:
	ActorFactory();
	
	StrongActorPtr CreateActor(const char* actorResource);

protected:
	virtual StrongActorComponentPtr ActorComponentCreator();

private:
	ActorId GetNextActorId() { ++m_lastActorId; return m_lastActorId; };
};

