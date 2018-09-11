#include "InputComponent.h"
#include "Command.h"

InputComponent::InputComponent(ComponentId componentId)
    : ActorComponent(componentId),
	  m_oldMousePos(Vector2D<int>(0, 0))
{
	// TODO: If same button could be used for multiple things (a powerup changes an action for example)
	//       an update method should be added to change command on button.
	//       Could also handle in base logic instead by having the same command do multiple things
	m_pButtonW = std::make_shared<MoveUp>();
	m_pButtonS = std::make_shared<MoveDown>();
	m_pButtonD = std::make_shared<MoveRight>();
	m_pButtonA = std::make_shared<MoveLeft>();
	m_pButtonEsc = nullptr;
	m_pButtonSpace = nullptr;
	m_pButtonReturn = nullptr;
	m_pMouseButtonRight = nullptr;
	m_pMouseButtonLeft = nullptr;
	m_pMouseButtonMiddle = nullptr;
	m_pMousePositionMoved = std::make_shared<UpdateMousePosition>();
}

InputComponent::~InputComponent()
{
}

void InputComponent::Update(GameActor& actor, float deltaMs)
{
	std::vector<std::shared_ptr<Command>> commandList;
	InputData input = actor.GetInput();
	if (input.buttonsPressed & EInputValues::W)
	{
		commandList.push_back(m_pButtonW);
	}
	if (input.buttonsPressed & EInputValues::A)
	{
		commandList.push_back(m_pButtonA);
	}
	if (input.buttonsPressed & EInputValues::S)
	{
		commandList.push_back(m_pButtonS);
	}
	if (input.buttonsPressed & EInputValues::D)
	{
		commandList.push_back(m_pButtonD);
	}
	if (input.buttonsPressed & EInputValues::Esc)
	{
		commandList.push_back(m_pButtonEsc);
	}
	if (input.buttonsPressed & EInputValues::Space)
	{
		commandList.push_back(m_pButtonSpace);
	}
	if (input.buttonsPressed & EInputValues::Return)
	{
		commandList.push_back(m_pButtonReturn);
	}
	if (input.buttonsPressed & EInputValues::MouseRight)
	{
		commandList.push_back(m_pMouseButtonRight);
	}
	if (input.buttonsPressed & EInputValues::MouseLeft)
	{
		commandList.push_back(m_pMouseButtonLeft);
	}
	if (input.buttonsPressed & EInputValues::MouseMiddle)
	{
		commandList.push_back(m_pMouseButtonMiddle);
	}
	if (input.mousePos != m_oldMousePos)
	{
		m_oldMousePos = input.mousePos;
		m_pMousePositionMoved->SetMousePosition(input.mousePos);
		commandList.push_back(m_pMousePositionMoved);
	}

	actor.SetCommands(std::make_shared<std::vector<std::shared_ptr<Command>>>(commandList));
}

const EComponentNames InputComponent::GetComponentName() const
{
	return EComponentNames::InputComponentEnum;
}