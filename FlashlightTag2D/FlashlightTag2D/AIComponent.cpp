#include "AIComponent.h"
#include "GameActor.h"
#include "Command.h"
#include <memory>

AIComponent::AIComponent()
{
	m_moveTimer = 0;
	m_moveDirection = 0;
	m_moveLength = 10;
}

AIComponent::~AIComponent()
{
}

void AIComponent::Update(GameActor& actor, int deltaMs)
{
	// If hiding
	//  Look for seeker in light cone
	//   Seeker found - run away
	//actor.SetCommands(std::make_shared<CommandList>(RunFromTarget()));
	//   Seeker not found - stay or change hiding space
	//actor.SetCommands(std::make_shared<CommandList>(Hide()));

	// If seeking
	//  Look for hider in light cone
	//   Hider found - chase hider
	//actor.SetCommands(std::make_shared<CommandList>(ChaseTarget()));
	//   Hider not found - use heuristic to search
	//actor.SetCommands(std::make_shared<CommandList>(SearchForTargets()));

	if (curBehavior != nullptr)
	{
		actor.SetCommands(std::make_shared<CommandList>(curBehavior->Update()));
	}
}

CommandList AIComponent::SimpleMove()
{
	CommandList commandList;
	if (m_moveTimer > 0)
	{
		m_moveTimer--;
	}
	else
	{
		if (m_moveLength < 0)
		{
			m_moveDirection = (m_moveDirection + 1) % 4;
			m_moveLength = 10;
		}
		else
		{
			m_moveLength--;
		}
		
		m_moveTimer = 300;

		switch (m_moveDirection)
		{
			case 0:
				commandList.push_back(std::make_shared<MoveDown>());
				break;
			case 1:
				commandList.push_back(std::make_shared<MoveRight>());
				break;
			case 2:
				commandList.push_back(std::make_shared<MoveUp>());
				break;
			case 3:
				commandList.push_back(std::make_shared<MoveLeft>());
				break;
		}
	}
	
	return commandList;
}

ComponentId AIComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames AIComponent::GetComponentName() const
{
	return EComponentNames::AIComponentEnum;
}