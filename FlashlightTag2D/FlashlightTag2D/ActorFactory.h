#pragma once
#include "document.h"

#include "Types.h"

class ActorFactory
{
public:
	ActorFactory();
	
    void CreateActorsFromJSONArray(const rapidjson::Value& actorList, GraphicsManager& graphicsMgr);
	StrongGameActorPtrList GetActorList() { return m_pEntityList; }

    // Move to graphics manager
    StrongGameActorPtr CreateCamera();
    StrongGameActorPtr CreateCamera(StrongGameActorPtr target);

private:
	ActorId m_lastActorId;
	ActorId get_next_actor_id() { ++m_lastActorId; return m_lastActorId; };
	StrongGameActorPtrList m_pEntityList;

    // Only needed for camera
    StrongGameActorPtr m_pCurrentPlayer;
};

