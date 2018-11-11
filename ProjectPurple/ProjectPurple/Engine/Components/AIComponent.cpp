#include "AIComponent.h"
#include "GameActor.h"
#include "GameStateComponent.h"
#include "Behavior.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"
#include "Command.h"

#include <memory>

AIComponent::AIComponent(ComponentId componentId, std::vector<std::shared_ptr<Behavior>> behaviorPtrVec)
    : ActorComponent(componentId),
      m_behaviorPtrVec(behaviorPtrVec)
{
}

AIComponent::~AIComponent()
{
}

void AIComponent::Update(GameActor& actor, float deltaMs)
{	
    if (!m_behaviorPtrVec.empty())
    {
        std::shared_ptr<std::vector<std::shared_ptr<Command>>> commandPtrVecPtr(std::make_shared<std::vector<std::shared_ptr<Command>>>());
        for (auto behavior : m_behaviorPtrVec)
        {
            auto cmdPtrVec = behavior->Update(actor);
            commandPtrVecPtr->insert(commandPtrVecPtr->end(), cmdPtrVec.begin(), cmdPtrVec.end());
        }

        actor.SetCommands(commandPtrVecPtr);
    }
}

const EComponentNames AIComponent::GetComponentName() const
{
    return EComponentNames::AIComponentEnum;
}