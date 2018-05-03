#include "ServiceLocator.h"

StrongActorFactoryPtr ServiceLocator::m_actorFactoryService = nullptr;
StrongInputManagerPtr ServiceLocator::m_inputManagerService = nullptr;
StrongPhysicsManagerPtr ServiceLocator::m_physicsManagerService = nullptr;
StrongGraphicsManagerPtr ServiceLocator::m_graphicsManagerService = nullptr;
std::shared_ptr<RandomNumberGenerator> ServiceLocator::m_randomNumberGenerator = nullptr;

