#include "CursorInputComponent.h"
#include "Command.h"

CursorInputComponent::CursorInputComponent(ComponentId componentId)
    :InputComponent(componentId)
{
    m_updateMousePositionPtr = std::make_shared<UpdateMousePosition>();
}

CursorInputComponent::~CursorInputComponent()
{
}
