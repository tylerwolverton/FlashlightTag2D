#include "LogicComponent.h"
#include "PhysicsComponent.h"

LogicComponent::LogicComponent(ComponentId componentId)
    : ActorComponent(componentId)
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