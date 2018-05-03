#pragma once
#include "Types.h"

class ActorFactory;
class InputManager;
class PhysicsManager;
class GraphicsManager;
class RandomNumberGenerator;

class ServiceLocator
{
public:
	static StrongActorFactoryPtr GetActorFactory() { return m_actorFactoryService; }
	static StrongInputManagerPtr GetInputManager() { return m_inputManagerService; }
	static StrongPhysicsManagerPtr GetPhysicsManager() { return m_physicsManagerService; }
	static StrongGraphicsManagerPtr GetGraphicsManager() { return m_graphicsManagerService; }
	static std::shared_ptr<RandomNumberGenerator> GetRandomNumberGenerator() { return m_randomNumberGenerator; }

	static void Provide(StrongActorFactoryPtr service)
	{
		m_actorFactoryService = service;
	}
	static void Provide(StrongInputManagerPtr service)
	{
		m_inputManagerService = service;
	}
	static void Provide(StrongPhysicsManagerPtr service)
	{
		m_physicsManagerService = service;
	}
	static void Provide(StrongGraphicsManagerPtr service)
	{
		m_graphicsManagerService = service;
	}
	static void Provide(std::shared_ptr<RandomNumberGenerator> service)
	{
		m_randomNumberGenerator = service;
	}

private:
	static StrongActorFactoryPtr m_actorFactoryService;
	static StrongInputManagerPtr m_inputManagerService;
	static StrongPhysicsManagerPtr m_physicsManagerService;
	static StrongGraphicsManagerPtr m_graphicsManagerService;
	static std::shared_ptr<RandomNumberGenerator> m_randomNumberGenerator;
};

