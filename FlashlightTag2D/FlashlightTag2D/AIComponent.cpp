#include "AIComponent.h"
#include "GameActor.h"
#include "GameStateComponent.h"
#include "Behavior.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"
#include "Command.h"

#include <memory>

AIComponent::AIComponent(ComponentId componentId, std::vector<std::shared_ptr<Behavior>> behaviorVec)
    : ActorComponent(componentId),
	  m_behaviorVec(behaviorVec)
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
    if (!m_behaviorVec.empty())
    {
        std::shared_ptr<std::vector<std::shared_ptr<Command>>> commandsVec(std::make_shared<std::vector<std::shared_ptr<Command>>>());
        for (auto behavior : m_behaviorVec)
        {
            auto cmds = behavior->Update(actor);
            commandsVec->insert(commandsVec->end(), cmds.begin(), cmds.end());
        }

        actor.SetCommands(commandsVec);
    }
}

const EComponentNames AIComponent::GetComponentName() const
{
	return EComponentNames::AIComponentEnum;
}