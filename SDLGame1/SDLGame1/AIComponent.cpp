#include "AIComponent.h"
#include "GameActor.h"
#include "Command.h"
#include <memory>

AIComponent::AIComponent()
{
	moveTimer = 0;
	moveDirection = 0;
	moveLength = 10;
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
		if (moveLength < 0)
		{
			moveDirection = (moveDirection + 1) % 4;
			moveLength = 10;
		}
		else
		{
			moveLength--;
		}
		
		moveTimer = 300;


		switch (moveDirection)
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