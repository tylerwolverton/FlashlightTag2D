#include "FollowTargetAIComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

FollowTargetAIComponent::FollowTargetAIComponent()
{
	m_pTarget = nullptr;
}

FollowTargetAIComponent::FollowTargetAIComponent(StrongGameActorPtr target)
	: m_pTarget(target)
{
}

FollowTargetAIComponent::~FollowTargetAIComponent()
{
}

void FollowTargetAIComponent::Update(GameActor& actor, int deltaMs) 
{
	auto actorTransformComponent = actor.GetTransformComponent();
	auto targetTransformComponent = m_pTarget->GetTransformComponent();
	if (actorTransformComponent == nullptr || targetTransformComponent == nullptr)
	{
		return;
	}

    actorTransformComponent->SetPosition(Vector2D<float>(targetTransformComponent->GetPosition().x - World::SCREEN_WIDTH / 2, targetTransformComponent->GetPosition().y - World::SCREEN_HEIGHT / 2));
}

ComponentId FollowTargetAIComponent::GetComponentId() const { return ComponentId(); }

EComponentNames FollowTargetAIComponent::GetComponentName() const
{
	return EComponentNames::FollowTargetAIComponentEnum;
}

void FollowTargetAIComponent::SetTargetActor(StrongGameActorPtr actor)
{
	m_pTarget = actor;
}