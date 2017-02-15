#include "FollowTargetAIComponent.h"
#include "GameActor.h"

FollowTargetAIComponent::FollowTargetAIComponent()
{
	target = nullptr;
}

FollowTargetAIComponent::FollowTargetAIComponent(const GameActor& p_target)
	: target(std::make_shared<GameActor>(p_target))
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


void FollowTargetAIComponent::SetTargetActor(const GameActor& actor)
{
	target = std::make_shared<GameActor>(actor);
}