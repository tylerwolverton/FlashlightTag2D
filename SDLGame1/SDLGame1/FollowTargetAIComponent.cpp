#include "FollowTargetAIComponent.h"
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
	actor.m_position = target->m_position;
}

ComponentId FollowTargetAIComponent::GetComponentId() const { return ComponentId(); }


void FollowTargetAIComponent::SetTargetActor(StrongGameActorPtr actor)
{
	target = actor;
}