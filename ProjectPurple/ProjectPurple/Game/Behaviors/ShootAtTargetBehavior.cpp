#include "ShootAtTargetBehavior.h"
#include "ActorFactory.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"

#include <SDL.h>

ShootAtTargetBehavior::ShootAtTargetBehavior(const std::shared_ptr<GameActor>& targetActorPtr, int cooldown)
    : m_targetActorPtr(targetActorPtr),
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
        auto actorFactoryPtr = ServiceLocator::GetActorFactory();
        auto thisTransformCompPtr = actor.GetTransformCompPtr();
        if (actorFactoryPtr != nullptr && m_targetActorPtr != nullptr
            && thisTransformCompPtr != nullptr)
        {
            auto targetTransformCompPtr = m_targetActorPtr->GetTransformCompPtr();
            if (targetTransformCompPtr != nullptr)
            {
                Vector2D<float> dirVec = (targetTransformCompPtr->GetPosition() - thisTransformCompPtr->GetPosition()).Normalize();
                actorFactoryPtr->CreateProjectile(thisTransformCompPtr->GetPosition() + (dirVec * 70), dirVec * 15.0f);
            }
        }
    }

    return std::vector<std::shared_ptr<Command>>();
}