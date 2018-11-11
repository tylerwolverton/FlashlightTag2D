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
    auto actorFactoryPtr = ServiceLocator::GetActorFactory();
    if (actorFactoryPtr == nullptr)
    {
        return;
    }

    auto healthBarPtr = actorFactoryPtr->GetFirstActorWithName("PlayerHealthBar");
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