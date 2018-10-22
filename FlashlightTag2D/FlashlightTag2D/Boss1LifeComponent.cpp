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
    auto actorFactory = ServiceLocator::GetActorFactory();
    if (actorFactory == nullptr)
    {
        return;
    }

    auto healthBar = actorFactory->GetFirstActorWithName("Boss1HealthBar");
    if (healthBar == nullptr)
    {
        return;
    }

    std::shared_ptr<TransformComponent> transformComp = healthBar->GetTransformComponent();

    float newX = transformComp->GetInitSize().x / m_maxHealth * m_health + 1;
    if (newX > 0)
    {
        transformComp->SetSize(Vector2D<float>(newX, transformComp->GetSize().y));
    }
}