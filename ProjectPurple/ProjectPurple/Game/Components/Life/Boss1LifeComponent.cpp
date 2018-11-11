#include "Boss1LifeComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "GameActor.h"
#include "TransformComponent.h"

#include <SDL.h>

Boss1LifeComponent::Boss1LifeComponent(ComponentId componentId, ActorId parentId, int health)
    : LifeComponent(componentId, parentId, health)
{
}

Boss1LifeComponent::~Boss1LifeComponent()
{
}

void Boss1LifeComponent::TakeDamage(int damage)
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - lastTickVal > 500)
    {
        lastTickVal = curTicks;

        m_health -= damage;
        updateHealthBar();

        if (m_health < 0)
        {
            Die();
        }
    }
}

void Boss1LifeComponent::updateHealthBar()
{
    auto actorFactoryPtr = ServiceLocator::GetActorFactory();
    if (actorFactoryPtr == nullptr)
    {
        return;
    }

    auto healthBarPtr = actorFactoryPtr->GetFirstActorWithName("Boss1HealthBar");
    if (healthBarPtr == nullptr)
    {
        return;
    }

    auto transformCompPtr = healthBarPtr->GetTransformCompPtr();

    float newX = transformCompPtr->GetInitSize().x / m_maxHealth * m_health + 1;
    if (newX > 0)
    {
        transformCompPtr->SetSize(Vector2D<float>(newX, transformCompPtr->GetSize().y));
    }
}