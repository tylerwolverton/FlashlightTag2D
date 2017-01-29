#include "AIComponent.h"
#include "GameActor.h"
#include "Command.h"
#include <memory>

AIComponent::AIComponent()
{
	moveTimer = 0;
	moveDirection = 0;
}


AIComponent::~AIComponent()
{
}

bool AIComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
}

void AIComponent::PostInit()
{
}

void AIComponent::Update(GameActor& actor, int deltaMs)
{
	actor.commands = std::make_shared<CommandList>(SimpleMove());
}

CommandList AIComponent::SimpleMove()
{
	CommandList commandList;
	if (moveTimer > 0)
	{
		moveTimer--;
	}
	else
	{
		moveDirection = (moveDirection + 1) % 2;
		moveTimer = 60;
	}

	switch (moveDirection)
	{
		case 0:
			commandList.push_back(std::make_shared<MoveDown>());
			break;
		case 1:
			commandList.push_back(std::make_shared<MoveUp>());
			break;
		case 2:
			break;
		case 3:
			break;
	}

	
	return commandList;
}

ComponentId AIComponent::GetComponentId() const
{
	return ComponentId();
}