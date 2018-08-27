#include "ServiceLocator.h"

std::shared_ptr<ActorFactory> ServiceLocator::m_actorFactoryService = nullptr;
std::shared_ptr<LevelFactory> ServiceLocator::m_levelFactoryService = nullptr;
std::shared_ptr<InputManager> ServiceLocator::m_inputManagerService = nullptr;
std::shared_ptr<PhysicsManager> ServiceLocator::m_physicsManagerService = nullptr;
std::shared_ptr<GraphicsManager> ServiceLocator::m_graphicsManagerService = nullptr;
std::shared_ptr<RandomNumberGenerator> ServiceLocator::m_randomNumberGenerator = nullptr;

