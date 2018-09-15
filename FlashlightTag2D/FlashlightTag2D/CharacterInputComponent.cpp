#include "CharacterInputComponent.h"
#include "Command.h"

CharacterInputComponent::CharacterInputComponent(ComponentId componentId)
	:InputComponent(componentId)
{
	m_pButtonW = std::make_shared<MoveUp>();
	m_pButtonS = std::make_shared<MoveDown>();
	m_pButtonD = std::make_shared<MoveRight>();
	m_pButtonA = std::make_shared<MoveLeft>();
	m_pMouseButtonLeft = std::make_shared<Shoot>();
	m_pMousePositionMoved = std::make_shared<UpdateMousePosition>();
}

CharacterInputComponent::~CharacterInputComponent()
{
}
