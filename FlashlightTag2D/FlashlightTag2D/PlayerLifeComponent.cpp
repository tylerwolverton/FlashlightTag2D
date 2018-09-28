#include "PlayerLifeComponent.h"

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
    if (curTicks - lastTickVal > 1000)
    {
        lastTickVal = curTicks;

        m_health -= damage;
        if (m_health < 0)
        {
            Die();
        }
    }
}