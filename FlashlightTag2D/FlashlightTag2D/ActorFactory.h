#pragma once
#include "document.h"

#include "Types.h"
#include "Vector2D.h"
#include <vector>

class ActorFactory
{
public:
	ActorFactory();
	
    void CreateActorsFromJSONArray(const rapidjson::Value& actorList, PhysicsManager& physicsMgr, GraphicsManager& graphicsMgr);
	StrongGameActorPtrList GetActorList() { return m_pEntityList; }
	void ChooseSeekers(int seekerCount);

    // Move to graphics manager
    StrongGameActorPtr CreateCamera(const Vector2D<int>& levelSize);
    StrongGameActorPtr CreateCamera(const StrongGameActorPtr& target, const Vector2D<int>& levelSize);

private:
	ActorId m_lastActorId;
	ActorId getNextActorId() { ++m_lastActorId; return m_lastActorId; };
    ComponentId m_lastComponentId;
    ComponentId getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };
	StrongGameActorPtrList m_pEntityList;
	
    // TODO: Cache changes
    //std::vector<GameActor> m_entityVec;

	std::vector<StrongGameStateComponentPtr> m_gameStateComponentVec;

    // Only needed for camera
    StrongGameActorPtr m_pCurrentPlayer;
};

