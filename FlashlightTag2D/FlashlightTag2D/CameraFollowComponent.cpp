#include "CameraFollowComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

CameraFollowComponent::CameraFollowComponent(ComponentId componentId)
    : ActorComponent(componentId)
{
	m_pTarget = nullptr;
}

CameraFollowComponent::CameraFollowComponent(ComponentId componentId, StrongGameActorPtr target, Vector2D<int> levelSize)
    : ActorComponent(componentId),
	  m_pTarget(target),
      m_levelSize(levelSize)
{
}

CameraFollowComponent::~CameraFollowComponent()
{
}

void CameraFollowComponent::Update(GameActor& actor, float deltaMs)
{
	StrongTransformComponentPtr actorTransformComponent = actor.GetTransformComponent();
    StrongTransformComponentPtr targetTransformComponent = m_pTarget->GetTransformComponent();
	if (actorTransformComponent == nullptr || targetTransformComponent == nullptr)
	{
		return;
	}

    Vector2D<float> newPosition(targetTransformComponent->GetPosition().x - World::SCREEN_WIDTH / 2,
                                targetTransformComponent->GetPosition().y - World::SCREEN_HEIGHT / 2);


    // if outside bounds of the level, move back in
    if (newPosition.x < 0)
    {
        newPosition.x = 0;
    }
    else if (newPosition.x + World::SCREEN_WIDTH > m_levelSize.x)
    {
        newPosition.x = m_levelSize.x - World::SCREEN_WIDTH;
    }
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + World::SCREEN_HEIGHT > m_levelSize.y)
    {
        newPosition.y = m_levelSize.y - World::SCREEN_HEIGHT;
    }

    actorTransformComponent->SetPosition(newPosition);
}

const EComponentNames CameraFollowComponent::GetComponentName() const
{
	return EComponentNames::CameraFollowComponentEnum;
}

void CameraFollowComponent::SetTargetActor(StrongGameActorPtr actor)
{
	m_pTarget = actor;
}