#include "LifeComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"

LifeComponent::LifeComponent(ComponentId componentId, ActorId parentId, int health)
    : ActorComponent(componentId),
      m_parentId(parentId),
      m_health(health)
{
}

LifeComponent::~LifeComponent()
{
}

void LifeComponent::Die()
{
    auto actorFactory = ServiceLocator::GetActorFactory();
    if (actorFactory == nullptr)
    {
        return;
    }

    actorFactory->AddDeadActor(m_parentId);
}

void LifeComponent::TakeDamage(int damage)
{
    m_health -= damage;

    if (m_health < 0)
    {
        Die();
    }
}