#include "ServiceLocator.h"

StrongActorFactoryPtr ServiceLocator::m_actorFactoryService = nullptr;
StrongInputManagerPtr ServiceLocator::m_inputManagerService = nullptr;
StrongPhysicsManagerPtr ServiceLocator::m_physicsManagerService = nullptr;
StrongGraphicsManagerPtr ServiceLocator::m_graphicsManagerService = nullptr;
//
//ServiceLocator::ServiceLocator()
//{
//	m_actorFactoryService = nullptr;
//}
//
//ServiceLocator::~ServiceLocator()
//{
//}
