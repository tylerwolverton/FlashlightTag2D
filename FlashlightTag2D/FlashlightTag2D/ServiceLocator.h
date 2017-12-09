#pragma once
class ActorFactory;

static class ServiceLocator
{
public:
	/*ServiceLocator();
	~ServiceLocator();*/

	static ActorFactory* GetActorFactory() { return m_actorFactoryService; }

	static void Provide(ActorFactory* service)
	{
		m_actorFactoryService = service;
	}

private:
	static ActorFactory* m_actorFactoryService;
};

