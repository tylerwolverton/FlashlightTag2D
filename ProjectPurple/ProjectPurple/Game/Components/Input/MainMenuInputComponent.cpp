#include "MainMenuInputComponent.h"
#include "Command.h"

MainMenuInputComponent::MainMenuInputComponent(ComponentId componentId)
    :InputComponent(componentId),
     maxTicks(50),
     elapsedTicks(50)
{
    m_wButtonCmdPtr = m_upButtonCmdPtr = std::make_shared<MoveUp>();
    m_sButtonCmdPtr = m_downButtonCmdPtr = std::make_shared<MoveDown>();
    m_spaceButtonCmdPtr = std::make_shared<Select>();
    m_returnButtonCmdPtr = std::make_shared<Select>();
    m_updateMousePositionPtr = std::make_shared<UpdateMousePosition>();
}

MainMenuInputComponent::~MainMenuInputComponent()
{
}