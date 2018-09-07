#include "LogicComponent.h"
#include "PhysicsComponent.h"

LogicComponent::LogicComponent(ComponentId componentId)//, std::shared_ptr<PhysicsComponent> physicsComponent)
	: ActorComponent(componentId)
      //m_pPhysicsComponent(physicsComponent)
{
}

LogicComponent::LogicComponent(int actorId, ComponentId componentId)
    : ActorComponent(componentId),
      m_actorId(actorId)
{
}

LogicComponent::~LogicComponent()
{
}

const EComponentNames LogicComponent::GetComponentName() const
{
	return EComponentNames::LogicComponentEnum;
}