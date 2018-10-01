#include "AIComponent.h"
#include "GameActor.h"
#include "GameStateComponent.h"
#include "Behavior.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"
#include "Command.h"

#include <memory>

AIComponent::AIComponent(ComponentId componentId, std::shared_ptr<Behavior> behavior)
    : ActorComponent(componentId),
	  m_behavior(behavior)
{
}

AIComponent::~AIComponent()
{
}

void AIComponent::Update(GameActor& actor, float deltaMs)
{	
	/*auto gameStateComponent = actor.GetGameStateComponent();
	if (gameStateComponent == nullptr)
	{
		return;
	}

	auto behavior = gameStateComponent->GetBehavior();*/
	if (m_behavior != nullptr)
	{
		actor.SetCommands(std::make_shared<std::vector<std::shared_ptr<Command>>>(m_behavior->Update(actor)));
	}
}

const EComponentNames AIComponent::GetComponentName() const
{
	return EComponentNames::AIComponentEnum;
}