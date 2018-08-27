#include "MainMenuInputComponent.h"
#include "Command.h"

MainMenuInputComponent::MainMenuInputComponent(ComponentId componentId)
	:InputComponent(componentId)
{
	m_pButtonW = std::make_shared<MoveUp>();
	m_pButtonS = std::make_shared<MoveDown>();
	m_pButtonSpace = std::make_shared<StartGame>();
	m_pButtonReturn = std::make_shared<StartGame>();
}

MainMenuInputComponent::~MainMenuInputComponent()
{
}
