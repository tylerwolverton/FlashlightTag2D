#pragma once
#include <memory>
#include <map>
#include <tinyxml2.h>

#include "SDLUtils.h"
#include "Types.h"

class ActorFactory
{
public:
	ActorFactory();
	
	StrongGameActorPtr CreatePlayer();
	StrongGameActorPtr CreateEnemy();
	StrongGameActorPtr CreateCamera(StrongGameActorPtr target);
	StrongGameActorPtrList GetActorList() { return m_pEntityList; }

protected:
	ActorComponentCreatorMap m_actorComponentCreators;
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

private:
	ActorId m_lastActorId;
	ActorId get_next_actor_id() { ++m_lastActorId; return m_lastActorId; };
	StrongGameActorPtrList m_pEntityList;
};

