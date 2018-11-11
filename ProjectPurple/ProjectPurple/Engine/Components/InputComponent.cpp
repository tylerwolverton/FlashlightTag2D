#include "InputComponent.h"
#include "Command.h"

InputComponent::InputComponent(ComponentId componentId)
    : ActorComponent(componentId),
      m_oldMousePos(Vector2D<int>(0, 0)),
      m_wButtonCmdPtr(nullptr),
      m_sButtonCmdPtr(nullptr),
      m_dButtonCmdPtr(nullptr),
      m_aButtonCmdPtr(nullptr),
      m_upButtonCmdPtr(nullptr),
      m_leftButtonPtr(nullptr),
      m_downButtonCmdPtr(nullptr),
      m_rightButtonCmdPtr(nullptr),
      m_escButtonCmdPtr(nullptr),
      m_spaceButtonCmdPtr(nullptr),
      m_returnButtonCmdPtr(nullptr),
      m_rightMouseButtonCmdPtr(nullptr),
      m_leftMouseButtonCmdPtr(nullptr),
      m_middleMouseButtonCmdPtr(nullptr),
      m_updateMousePositionPtr(nullptr)
{
}

InputComponent::~InputComponent()
{
}

void InputComponent::Update(GameActor& actor, float deltaMs)
{
    std::vector<std::shared_ptr<Command>> commandPtrVec;
    InputData input = actor.GetInput();
    if (input.buttonsPressed & EInputValues::W)
    {
        commandPtrVec.push_back(m_wButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::A)
    {
        commandPtrVec.push_back(m_aButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::S)
    {
        commandPtrVec.push_back(m_sButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::D)
    {
        commandPtrVec.push_back(m_dButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::Up)
    {
        commandPtrVec.push_back(m_upButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::Left)
    {
        commandPtrVec.push_back(m_leftButtonPtr);
    }
    if (input.buttonsPressed & EInputValues::Down)
    {
        commandPtrVec.push_back(m_downButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::Right)
    {
        commandPtrVec.push_back(m_rightButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::Esc)
    {
        commandPtrVec.push_back(m_escButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::Space)
    {
        commandPtrVec.push_back(m_spaceButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::Return)
    {
        commandPtrVec.push_back(m_returnButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::MouseRight)
    {
        commandPtrVec.push_back(m_rightMouseButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::MouseLeft)
    {
        commandPtrVec.push_back(m_leftMouseButtonCmdPtr);
    }
    if (input.buttonsPressed & EInputValues::MouseMiddle)
    {
        commandPtrVec.push_back(m_middleMouseButtonCmdPtr);
    }

    m_oldMousePos = input.mousePos;
    m_updateMousePositionPtr->SetMousePosition(input.mousePos);
    commandPtrVec.push_back(m_updateMousePositionPtr);

    actor.SetCommands(std::make_shared<std::vector<std::shared_ptr<Command>>>(commandPtrVec));
}

const EComponentNames InputComponent::GetComponentName() const
{
    return EComponentNames::InputComponentEnum;
}