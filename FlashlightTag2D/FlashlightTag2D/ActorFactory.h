#pragma once
#include "document.h"

#include "Types.h"

class ActorFactory
{
public:
	ActorFactory();
	
    void CreateActorsFromJSONArray(const rapidjson::Value& actorList, PhysicsManager& physicsMgr, GraphicsManager& graphicsMgr);
	StrongGameActorPtrList GetActorList() { return m_pEntityList; }

    // Move to graphics manager
    StrongGameActorPtr CreateCamera();
    StrongGameActorPtr CreateCamera(const StrongGameActorPtr& target);

private:
	ActorId m_lastActorId;
	ActorId getNextActorId() { ++m_lastActorId; return m_lastActorId; };
    ComponentId m_lastComponentId;
    ComponentId getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };
	StrongGameActorPtrList m_pEntityList;

    // Only needed for camera
    StrongGameActorPtr m_pCurrentPlayer;
};

