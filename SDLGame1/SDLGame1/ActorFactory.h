#pragma once
#include <memory>
#include <map>

#include "ActorComponent.h"
#include "Actor.h"

typedef ActorComponent *(*ActorComponentCreator)();
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

typedef unsigned long ActorId;
typedef unsigned long ComponentId;
typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;

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

