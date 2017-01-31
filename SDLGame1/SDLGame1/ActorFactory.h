#pragma once
#include <memory>
#include <map>
#include <tinyxml2.h>

#include "SDLUtils.h"
#include "Types.h"

class ActorFactory
{
	ActorId m_lastActorId;

protected:
	ActorComponentCreatorMap m_actorComponentCreators;
	GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

public:
	SDL_Renderer* m_renderer;

	ActorFactory(SDL_Renderer* renderer);
	
	StrongActorPtr CreateActor(const char* actorResource);
	StrongGameActorPtr CreatePlayer();
	StrongGameActorPtr CreateEnemy();

	virtual StrongActorComponentPtr VCreateComponent(tinyxml2::XMLElement* pData);

protected:
	virtual StrongActorComponentPtr ActorComponentCreator();

private:
	ActorId GetNextActorId() { ++m_lastActorId; return m_lastActorId; };
};
