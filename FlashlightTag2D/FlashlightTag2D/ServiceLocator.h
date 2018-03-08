#pragma once
#include "Types.h"

class ActorFactory;

class ServiceLocator
{
public:
	/*ServiceLocator();
	~ServiceLocator();*/

	static StrongActorFactoryPtr GetActorFactory() { return m_actorFactoryService; }

	static void Provide(StrongActorFactoryPtr service)
	{
		m_actorFactoryService = service;
	}

private:
	static StrongActorFactoryPtr m_actorFactoryService;
};

