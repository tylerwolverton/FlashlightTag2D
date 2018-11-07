#include "MainMenuInputComponent.h"
#include "Command.h"

MainMenuInputComponent::MainMenuInputComponent(ComponentId componentId)
    :InputComponent(componentId),
     maxTicks(50),
     elapsedTicks(50)
{
    m_pButtonW = m_pButtonUp = std::make_shared<MoveUp>();
    m_pButtonS = m_pButtonDown = std::make_shared<MoveDown>();
    m_pButtonSpace = std::make_shared<Select>();
    m_pButtonReturn = std::make_shared<Select>();
    m_pMousePositionMoved = std::make_shared<UpdateMousePosition>();
}

MainMenuInputComponent::~MainMenuInputComponent()
{
}

//void MainMenuInputComponent::Update(GameActor& actor, float deltaMs)
//{
//	elapsedTicks++;
//	if (elapsedTicks>= maxTicks)
//	{
//		InputComponent::Update(actor, deltaMs);
//		elapsedTicks = 0;
//	}
//}