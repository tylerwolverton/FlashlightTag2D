#include "CharacterInputComponent.h"
#include "Command.h"

CharacterInputComponent::CharacterInputComponent(ComponentId componentId)
	:InputComponent(componentId)
{
	m_pButtonW = m_pButtonUp = std::make_shared<MoveUp>();
    m_pButtonS = m_pButtonDown = std::make_shared<MoveDown>();
    m_pButtonD = m_pButtonRight = std::make_shared<MoveRight>();
    m_pButtonA = m_pButtonLeft = std::make_shared<MoveLeft>();
	m_pMouseButtonLeft = std::make_shared<Shoot>();
	m_pMousePositionMoved = std::make_shared<UpdateMousePosition>();
}

CharacterInputComponent::~CharacterInputComponent()
{
}
