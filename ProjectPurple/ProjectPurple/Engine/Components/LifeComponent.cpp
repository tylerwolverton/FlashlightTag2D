#include "LifeComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"

LifeComponent::LifeComponent(ComponentId componentId, ActorId parentId, int health)
    : ActorComponent(componentId),
      m_parentId(parentId),
      m_health(health),
      m_maxHealth(health)
{
}

LifeComponent::~LifeComponent()
{
}

const EComponentNames LifeComponent::GetComponentName() const
{
    return EComponentNames::LifeComponentEnum;
}

void LifeComponent::Die()
{
    auto actorFactoryPtr = ServiceLocator::GetActorFactory();
    if (actorFactoryPtr == nullptr)
    {
        return;
    }

    actorFactoryPtr->AddDeadActor(m_parentId);
}

void LifeComponent::TakeDamage(int damage)
{
    m_health -= damage;

    if (m_health < 0)
    {
        Die();
    }
}