#include "InputComponent.h"

InputComponent::InputComponent()
{
}


InputComponent::~InputComponent()
{
}

bool InputComponent::VInit(tinyxml2::XMLElement* pData)
{
	return true;
}

void InputComponent::VPostInit()
{
}

void InputComponent::VUpdate(int deltaMs)
{

}

ComponentId InputComponent::VGetComponentId() const
{
	return ComponentId();
}