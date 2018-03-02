#pragma once
#include <memory>
#include <map>
#include <tinyxml2.h>
#include "document.h"

#include "SDLUtils.h"
#include "Vector2D.h"
#include "Types.h"
#include "GameTypes.h"

class GameStateComponent;

class ActorFactory
{
public:
	ActorFactory();
	
    void CreateActorsFromJSONArray(const rapidjson::Value& actorList);
	StrongGameActorPtr CreatePlayer();
	StrongGameActorPtr CreateEnemy(Vector2D<float> position, EGameRole role);
	StrongGameActorPtr CreateCamera();
    StrongGameActorPtr CreateCamera(StrongGameActorPtr target);
	StrongGameActorPtrList GetActorList() { return m_pEntityList; }

protected:
	ActorComponentCreatorMap m_actorComponentCreators;
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

private:
	ActorId m_lastActorId;
	ActorId get_next_actor_id() { ++m_lastActorId; return m_lastActorId; };
	StrongGameActorPtrList m_pEntityList;
    StrongGameActorPtr m_pCurrentPlayer;
};

