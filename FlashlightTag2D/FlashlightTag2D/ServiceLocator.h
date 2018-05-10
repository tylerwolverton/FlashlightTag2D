#pragma once
#include <memory>

class ActorFactory;
class InputManager;
class PhysicsManager;
class GraphicsManager;
class RandomNumberGenerator;

class ServiceLocator
{
public:
	static std::shared_ptr<ActorFactory> GetActorFactory() { return m_actorFactoryService; }
	static std::shared_ptr<InputManager> GetInputManager() { return m_inputManagerService; }
	static std::shared_ptr<PhysicsManager> GetPhysicsManager() { return m_physicsManagerService; }
	static std::shared_ptr<GraphicsManager> GetGraphicsManager() { return m_graphicsManagerService; }
	static std::shared_ptr<RandomNumberGenerator> GetRandomNumberGenerator() { return m_randomNumberGenerator; }

	static void Provide(std::shared_ptr<ActorFactory> service)
	{
		m_actorFactoryService = service;
	}
	static void Provide(std::shared_ptr<InputManager> service)
	{
		m_inputManagerService = service;
	}
	static void Provide(std::shared_ptr<PhysicsManager> service)
	{
		m_physicsManagerService = service;
	}
	static void Provide(std::shared_ptr<GraphicsManager> service)
	{
		m_graphicsManagerService = service;
	}
	static void Provide(std::shared_ptr<RandomNumberGenerator> service)
	{
		m_randomNumberGenerator = service;
	}

private:
	static std::shared_ptr<ActorFactory> m_actorFactoryService;
	static std::shared_ptr<InputManager> m_inputManagerService;
	static std::shared_ptr<PhysicsManager> m_physicsManagerService;
	static std::shared_ptr<GraphicsManager> m_graphicsManagerService;
	static std::shared_ptr<RandomNumberGenerator> m_randomNumberGenerator;
};

