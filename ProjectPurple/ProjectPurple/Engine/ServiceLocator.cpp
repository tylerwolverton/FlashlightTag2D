#include "ServiceLocator.h"

std::shared_ptr<World> ServiceLocator::m_worldServicePtr = nullptr;
std::shared_ptr<ActorFactory> ServiceLocator::m_actorFactoryServicePtr = nullptr;
std::shared_ptr<LevelFactory> ServiceLocator::m_levelFactoryServicePtr = nullptr;
std::shared_ptr<GraphicsManager> ServiceLocator::m_graphicsManagerServicePtr = nullptr;
std::shared_ptr<RandomNumberGenerator> ServiceLocator::m_randomNumberGeneratorPtr = nullptr;

