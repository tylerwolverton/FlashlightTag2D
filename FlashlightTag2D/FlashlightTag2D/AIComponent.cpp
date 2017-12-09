#include "AIComponent.h"
#include "GameActor.h"
#include "GameStateComponent.h"
#include "Behavior.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"
#include "Command.h"
#include <memory>

AIComponent::AIComponent()
{
}

AIComponent::~AIComponent()
{
}

void AIComponent::Update(GameActor& actor, int deltaMs)
{
	// If hiding
	//  Look for seeker in light cone
	/*auto actorList = ServiceLocator::GetActorFactory()->GetActorList();*/
	//   Seeker found - run away
	//actor.SetCommands(std::make_shared<CommandList>(RunFromTarget()));
	//   Seeker not found - stay or change hiding space
	//actor.SetCommands(std::make_shared<CommandList>(Hide()));
	
	auto gameStateComponent = actor.GetGameStateComponent();
	if (gameStateComponent == nullptr)
	{
		return;
	}

	auto behavior = gameStateComponent->GetBehavior();
	if (behavior != nullptr)
	{
		actor.SetCommands(std::make_shared<CommandList>(behavior->Update(actor)));
	}
}

ComponentId AIComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames AIComponent::GetComponentName() const
{
	return EComponentNames::AIComponentEnum;
}