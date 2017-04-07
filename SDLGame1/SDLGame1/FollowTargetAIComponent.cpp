#include "FollowTargetAIComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"

FollowTargetAIComponent::FollowTargetAIComponent()
{
	target = nullptr;
}

FollowTargetAIComponent::FollowTargetAIComponent(StrongGameActorPtr p_target)
	: target(p_target)
{
}

FollowTargetAIComponent::~FollowTargetAIComponent()
{
}

bool FollowTargetAIComponent::Init(tinyxml2::XMLElement* pData) { return true; }
void FollowTargetAIComponent::PostInit() {}

void FollowTargetAIComponent::Update(GameActor& actor, int deltaMs) 
{
	StrongActorComponentPtr actorTransformComponent = actor.GetComponentByName(EComponentNames::TransformComponentEnum);
	StrongActorComponentPtr targetTransformComponent = target->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (actorTransformComponent == nullptr || targetTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actorTransformComponent);
	std::shared_ptr<TransformComponent> rawTargetTransformComponent = std::dynamic_pointer_cast<TransformComponent>(targetTransformComponent);
	rawActorTransformComponent->SetPosition(rawTargetTransformComponent->GetPosition());
}

ComponentId FollowTargetAIComponent::GetComponentId() const { return ComponentId(); }

EComponentNames FollowTargetAIComponent::GetComponentName() const
{
	return EComponentNames::FollowTargetAIComponentEnum;
}

void FollowTargetAIComponent::SetTargetActor(StrongGameActorPtr actor)
{
	target = actor;
}