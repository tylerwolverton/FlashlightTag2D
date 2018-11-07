#pragma once
#include <memory>

class World;
class ActorFactory;
class LevelFactory;
class InputManager;
class PhysicsManager;
class GraphicsManager;
class RandomNumberGenerator;

class ServiceLocator
{
public:
    static std::shared_ptr<World> GetWorld() { return m_worldService; }
    static std::shared_ptr<ActorFactory> GetActorFactory() { return m_actorFactoryService; }
    static std::shared_ptr<LevelFactory> GetLevelFactory() { return m_levelFactoryService; }
    //static std::shared_ptr<InputManager> GetInputManager() { return m_inputManagerService; }
    //static std::shared_ptr<PhysicsManager> GetPhysicsManager() { return m_physicsManagerService; }
    static std::shared_ptr<GraphicsManager> GetGraphicsManager() { return m_graphicsManagerService; }
    static std::shared_ptr<RandomNumberGenerator> GetRandomNumberGenerator() { return m_randomNumberGenerator; }

    static void Provide(std::shared_ptr<World> service)
    {
        m_worldService = service;
    }
    static void Provide(std::shared_ptr<ActorFactory> service)
    {
        m_actorFactoryService = service;
    }
    static void Provide(std::shared_ptr<LevelFactory> service)
    {
        m_levelFactoryService = service;
    }
    //static void Provide(std::shared_ptr<InputManager> service)
    //{
    //	m_inputManagerService = service;
    //}
    //static void Provide(std::shared_ptr<PhysicsManager> service)
    //{
    //	m_physicsManagerService = service;
    //}
    static void Provide(std::shared_ptr<GraphicsManager> service)
    {
        m_graphicsManagerService = service;
    }
    static void Provide(std::shared_ptr<RandomNumberGenerator> service)
    {
        m_randomNumberGenerator = service;
    }

private:
    static std::shared_ptr<World> m_worldService;
    static std::shared_ptr<ActorFactory> m_actorFactoryService;
    static std::shared_ptr<LevelFactory> m_levelFactoryService;
    //static std::shared_ptr<InputManager> m_inputManagerService;
    //static std::shared_ptr<PhysicsManager> m_physicsManagerService;
    static std::shared_ptr<GraphicsManager> m_graphicsManagerService;
    static std::shared_ptr<RandomNumberGenerator> m_randomNumberGenerator;
};

