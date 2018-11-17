#pragma once
#include <memory>

class World;
class ActorFactory;
class LevelFactory;
class GraphicsManager;
class RandomNumberGenerator;

class ServiceLocator
{
public:
    static std::shared_ptr<World> GetWorld() { return m_worldServicePtr; }
    static std::shared_ptr<ActorFactory> GetActorFactory() { return m_actorFactoryServicePtr; }
    static std::shared_ptr<LevelFactory> GetLevelFactory() { return m_levelFactoryServicePtr; }
    static std::shared_ptr<GraphicsManager> GetGraphicsManager() { return m_graphicsManagerServicePtr; }
    static std::shared_ptr<RandomNumberGenerator> GetRandomNumberGenerator() { return m_randomNumberGeneratorPtr; }

    static void Provide(const std::shared_ptr<World>& service)
    {
        m_worldServicePtr = service;
    }
    static void Provide(const std::shared_ptr<ActorFactory>& service)
    {
        m_actorFactoryServicePtr = service;
    }
    static void Provide(const std::shared_ptr<LevelFactory>& service)
    {
        m_levelFactoryServicePtr = service;
    }
    static void Provide(const std::shared_ptr<GraphicsManager>& service)
    {
        m_graphicsManagerServicePtr = service;
    }
    static void Provide(const std::shared_ptr<RandomNumberGenerator>& service)
    {
        m_randomNumberGeneratorPtr = service;
    }

private:
    static std::shared_ptr<World> m_worldServicePtr;
    static std::shared_ptr<ActorFactory> m_actorFactoryServicePtr;
    static std::shared_ptr<LevelFactory> m_levelFactoryServicePtr;
    static std::shared_ptr<GraphicsManager> m_graphicsManagerServicePtr;
    static std::shared_ptr<RandomNumberGenerator> m_randomNumberGeneratorPtr;
};

