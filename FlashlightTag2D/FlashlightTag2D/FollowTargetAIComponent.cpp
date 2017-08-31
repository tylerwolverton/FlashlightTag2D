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
	StrongActorComponentPtr actorTransformComponent = actor.GetComponentByName(EComponentNames::TransformComponentEnum);
	StrongActorComponentPtr targetTransformComponent = m_pTarget->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (actorTransformComponent == nullptr || targetTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actorTransformComponent);
	std::shared_ptr<TransformComponent> rawTargetTransformComponent = std::dynamic_pointer_cast<TransformComponent>(targetTransformComponent);
	rawActorTransformComponent->SetPosition(Vector2D<float>(rawTargetTransformComponent->GetPosition().x - World::SCREEN_WIDTH / 2, rawTargetTransformComponent->GetPosition().y - World::SCREEN_HEIGHT / 2));
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