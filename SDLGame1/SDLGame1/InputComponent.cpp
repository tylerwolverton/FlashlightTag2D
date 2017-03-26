#include "InputComponent.h"
#include "GameActor.h"
#include "Command.h"
#include <list>

InputComponent::InputComponent()
{
	buttonW = std::make_shared<MoveUp>();
	buttonS = std::make_shared<MoveDown>();
	buttonD = std::make_shared<MoveRight>();
	buttonA = std::make_shared<MoveLeft>();
}

InputComponent::~InputComponent()
{
}

bool InputComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
}

void InputComponent::PostInit()
{
}

void InputComponent::Update(GameActor& actor, int deltaMs)
{
	CommandList commandList;

	if (actor.m_input & EInputValues::W)
	{
		commandList.push_back(buttonW);
	}
	if (actor.m_input & EInputValues::A)
	{
		commandList.push_back(buttonA);
	}
	if (actor.m_input & EInputValues::S)
	{
		commandList.push_back(buttonS);
	}
	if (actor.m_input & EInputValues::D)
	{
		commandList.push_back(buttonD);
	}
	if (actor.m_input & EInputValues::Esc)
	{
		commandList.push_back(buttonEsc);
	}

	actor.commands = std::make_shared<CommandList>(commandList);
}

ComponentId InputComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames InputComponent::GetComponentName() const
{
	return EComponentNames::InputComponentEnum;
}