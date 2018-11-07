#include "PlayerLifeComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "GameActor.h"
#include "TransformComponent.h"

#include <SDL.h>

PlayerLifeComponent::PlayerLifeComponent(ComponentId componentId, ActorId parentId, int health)
    : LifeComponent(componentId, parentId, health)
{
}

PlayerLifeComponent::~PlayerLifeComponent()
{
}

void PlayerLifeComponent::TakeDamage(int damage)
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > 500)
    {
        m_lastTickVal = curTicks;

        m_health -= damage;
        if (m_health < 0)
        {
            Die();
        }

        UpdateHealthBar();
    }
}

void PlayerLifeComponent::UpdateHealthBar()
{
    auto actorFactory = ServiceLocator::GetActorFactory();
    if (actorFactory == nullptr)
    {
        return;
    }

    auto healthBar = actorFactory->GetFirstActorWithName("PlayerHealthBar");
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