#include "InputComponent.h"
#include "Command.h"

InputComponent::InputComponent(ComponentId componentId)
    : ActorComponent(componentId)
{
	// TODO: If same button could be used for multiple things (a powerup changes an action for example)
	//       an update method should be added to change command on button.
	//       Could also handle in base logic instead by having the same command do multiple things
	m_pButtonW = std::make_shared<MoveUp>();
	m_pButtonS = std::make_shared<MoveDown>();
	m_pButtonD = std::make_shared<MoveRight>();
	m_pButtonA = std::make_shared<MoveLeft>();
}

InputComponent::~InputComponent()
{
}

void InputComponent::Update(GameActor& actor, float deltaMs)
{
	CommandList commandList;

	if (actor.GetInput() & EInputValues::W)
	{
		commandList.push_back(m_pButtonW);
	}
	if (actor.GetInput() & EInputValues::A)
	{
		commandList.push_back(m_pButtonA);
	}
	if (actor.GetInput() & EInputValues::S)
	{
		commandList.push_back(m_pButtonS);
	}
	if (actor.GetInput() & EInputValues::D)
	{
		commandList.push_back(m_pButtonD);
	}
	if (actor.GetInput() & EInputValues::Esc)
	{
		commandList.push_back(m_pButtonEsc);
	}

	actor.SetCommands(std::make_shared<CommandList>(commandList));
}

EComponentNames InputComponent::GetComponentName() const
{
	return EComponentNames::InputComponentEnum;
}