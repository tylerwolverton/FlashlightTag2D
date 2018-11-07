#include "ShootAtTargetBehavior.h"
#include "ActorFactory.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"

#include <SDL.h>

ShootAtTargetBehavior::ShootAtTargetBehavior(std::shared_ptr<GameActor> targetActor, int cooldown)
    : m_targetActor(targetActor),
      m_cooldown(cooldown)
{
}

ShootAtTargetBehavior::~ShootAtTargetBehavior()
{
}

std::vector<std::shared_ptr<Command>> ShootAtTargetBehavior::Update(const GameActor& actor)
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > m_cooldown * 1000)
    {
        m_lastTickVal = curTicks;
        auto actorFactory = ServiceLocator::GetActorFactory();
        auto thisTransformComp = actor.GetTransformComponent();
        if (actorFactory != nullptr && m_targetActor != nullptr
            && thisTransformComp != nullptr)
        {
            auto targetTransformComp = m_targetActor->GetTransformComponent();
            if (targetTransformComp != nullptr)
            {
                Vector2D<float> dirVec = (targetTransformComp->GetPosition() - thisTransformComp->GetPosition()).Normalize();
                actorFactory->CreateProjectile(thisTransformComp->GetPosition() + (dirVec * 70), dirVec * 15.0f);
            }
        }
    }

    return std::vector<std::shared_ptr<Command>>();
}