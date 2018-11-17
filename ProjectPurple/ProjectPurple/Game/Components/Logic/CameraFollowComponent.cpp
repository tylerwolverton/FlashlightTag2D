#include "CameraFollowComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

CameraFollowComponent::CameraFollowComponent(ComponentId componentId)
    : ActorComponent(componentId)
{
    m_targetActorPtr = nullptr;
}

CameraFollowComponent::CameraFollowComponent(ComponentId componentId, 
                                             const std::shared_ptr<GameActor>& targetActorPtr, 
                                             const Vector2D<int>& levelSize)
    : ActorComponent(componentId),
      m_targetActorPtr(targetActorPtr),
      m_levelSize(levelSize)
{
}

CameraFollowComponent::~CameraFollowComponent()
{
}

void CameraFollowComponent::Update(GameActor& actor, float deltaMs)
{
    if (m_targetActorPtr == nullptr)
    {
        return;
    }

    auto actorTransformCompPtr = actor.GetTransformCompPtr();
    auto targetTransformCompPtr = m_targetActorPtr->GetTransformCompPtr();
    if (actorTransformCompPtr == nullptr || targetTransformCompPtr == nullptr)
    {
        return;
    }

    Vector2D<float> newPosition(targetTransformCompPtr->GetPosition().x - World::SCREEN_WIDTH / 2,
                                targetTransformCompPtr->GetPosition().y - World::SCREEN_HEIGHT / 2);


    // if outside bounds of the level, move back in
    if (World::SCREEN_WIDTH > m_levelSize.x)
    {
        newPosition.x = -(World::SCREEN_WIDTH - m_levelSize.x) / 2.0f;
    }
    else
    {
        if (newPosition.x < 0)
        {
            newPosition.x = 0;
        }
        else if (newPosition.x + World::SCREEN_WIDTH > m_levelSize.x)
        {
            newPosition.x = (float)(m_levelSize.x - World::SCREEN_WIDTH);
        }
    }

    if (World::SCREEN_HEIGHT > m_levelSize.y)
    {
        newPosition.y = -(World::SCREEN_HEIGHT - m_levelSize.y) / 2.0f;
    }
    else
    {
        if (newPosition.y < 0)
        {
            newPosition.y = 0;
        }
        else if (newPosition.y + World::SCREEN_HEIGHT > m_levelSize.y)
        {
            newPosition.y = (float)(m_levelSize.y - World::SCREEN_HEIGHT);
        }
    }

    actorTransformCompPtr->SetPosition(newPosition);
}

const EComponentNames CameraFollowComponent::GetComponentName() const
{
    return EComponentNames::CameraFollowComponentEnum;
}

void CameraFollowComponent::SetTargetActor(const std::shared_ptr<GameActor>& actor)
{
    m_targetActorPtr = actor;
}