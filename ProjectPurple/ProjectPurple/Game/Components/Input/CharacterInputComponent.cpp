#include "CharacterInputComponent.h"
#include "Command.h"

CharacterInputComponent::CharacterInputComponent(ComponentId componentId)
    :InputComponent(componentId)
{
    m_wButtonCmdPtr = m_upButtonCmdPtr = std::make_shared<MoveUp>();
    m_sButtonCmdPtr = m_downButtonCmdPtr = std::make_shared<MoveDown>();
    m_dButtonCmdPtr = m_rightButtonCmdPtr = std::make_shared<MoveRight>();
    m_aButtonCmdPtr = m_leftButtonPtr = std::make_shared<MoveLeft>();
    m_leftMouseButtonCmdPtr = std::make_shared<Shoot>();
    m_updateMousePositionPtr = std::make_shared<UpdateMousePosition>();
}

CharacterInputComponent::~CharacterInputComponent()
{
}
