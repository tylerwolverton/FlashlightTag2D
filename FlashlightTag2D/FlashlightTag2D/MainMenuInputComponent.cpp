#include "MainMenuInputComponent.h"
#include "Command.h"

MainMenuInputComponent::MainMenuInputComponent(ComponentId componentId)
	:InputComponent(componentId),
	 maxTicks(50),
	 elapsedTicks(50)
{
	m_pButtonW = std::make_shared<MoveUp>();
	m_pButtonS = std::make_shared<MoveDown>();
	m_pButtonSpace = std::make_shared<StartGame>();
	m_pButtonReturn = std::make_shared<StartGame>();
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