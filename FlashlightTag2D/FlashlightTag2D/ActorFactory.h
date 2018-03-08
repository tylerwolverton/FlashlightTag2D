#pragma once
#include "document.h"

#include "Types.h"

class ActorFactory
{
public:
	ActorFactory();
	
    void CreateActorsFromJSONArray(const rapidjson::Value& actorList);
	StrongGameActorPtr CreateCamera();
    StrongGameActorPtr CreateCamera(StrongGameActorPtr target);
	StrongGameActorPtrList GetActorList() { return m_pEntityList; }

private:
	ActorId m_lastActorId;
	ActorId get_next_actor_id() { ++m_lastActorId; return m_lastActorId; };
	StrongGameActorPtrList m_pEntityList;
    StrongGameActorPtr m_pCurrentPlayer;
};

