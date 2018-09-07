#include <SDL.h>

#include "MainMenuLogicComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "LevelFactory.h"

MainMenuLogicComponent::MainMenuLogicComponent(ComponentId componentId, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<GraphicsComponent>>> buttonGraphicsCompMapPtr)
	: LogicComponent(componentId),
	  m_buttonGraphicsCompMapPtr(buttonGraphicsCompMapPtr),
	  selectedButtonIdx(0)
{
	buttons.push_back(EMenuButtons::Play);
	buttons.push_back(EMenuButtons::Controls);
	buttons.push_back(EMenuButtons::Exit);
}

MainMenuLogicComponent::~MainMenuLogicComponent()
{
}

void MainMenuLogicComponent::MoveUp()
{
	uint32_t curTicks = SDL_GetTicks();
	if (curTicks - lastTickVal > 200)
	{
		lastTickVal = curTicks;
		if (buttons[selectedButtonIdx] == EMenuButtons::Play)
		{
			// Play unselect
			m_buttonGraphicsCompMapPtr->find("Play")->second->SetTexture("resources/MainMenu/Play.png");
			// Exit select
			m_buttonGraphicsCompMapPtr->find("Exit")->second->SetTexture("resources/MainMenu/ExitSelected.png");
		}
		else if (buttons[selectedButtonIdx] == EMenuButtons::Controls)
		{
			// Controls unselect
			m_buttonGraphicsCompMapPtr->find("Controls")->second->SetTexture("resources/MainMenu/Controls.png");
			// Play select
			m_buttonGraphicsCompMapPtr->find("Play")->second->SetTexture("resources/MainMenu/PlaySelected.png");
		}
		else
		{
			// Exit unselect
			m_buttonGraphicsCompMapPtr->find("Exit")->second->SetTexture("resources/MainMenu/Exit.png");
			// Controls select
			m_buttonGraphicsCompMapPtr->find("Controls")->second->SetTexture("resources/MainMenu/ControlsSelected.png");
		}

		selectedButtonIdx--;
		if (selectedButtonIdx < 0)
		{
			selectedButtonIdx = buttons.size() - 1;
		}
	}
}

void MainMenuLogicComponent::MoveDown()
{
	uint32_t curTicks = SDL_GetTicks();
	if (curTicks - lastTickVal > 200)
	{
		lastTickVal = curTicks;
		if (buttons[selectedButtonIdx] == EMenuButtons::Play)
		{
			// Play unselect
			m_buttonGraphicsCompMapPtr->find("Play")->second->SetTexture("resources/MainMenu/Play.png");
			// Controls select
			m_buttonGraphicsCompMapPtr->find("Controls")->second->SetTexture("resources/MainMenu/ControlsSelected.png");
		}
		else if (buttons[selectedButtonIdx] == EMenuButtons::Controls)
		{
			// Controls unselect
			m_buttonGraphicsCompMapPtr->find("Controls")->second->SetTexture("resources/MainMenu/Controls.png");
			// Exit select
			m_buttonGraphicsCompMapPtr->find("Exit")->second->SetTexture("resources/MainMenu/ExitSelected.png");
		}
		else
		{
			// Exit unselect
			m_buttonGraphicsCompMapPtr->find("Exit")->second->SetTexture("resources/MainMenu/Exit.png");
			// Play select
			m_buttonGraphicsCompMapPtr->find("Play")->second->SetTexture("resources/MainMenu/PlaySelected.png");
		}


		selectedButtonIdx++;
		if (selectedButtonIdx >= buttons.size())
		{
			selectedButtonIdx = 0;
		}
	}
}

void MainMenuLogicComponent::StartGame()
{
	if (buttons[selectedButtonIdx] == EMenuButtons::Play)
	{
		std::shared_ptr<LevelFactory> levelFactory = ServiceLocator::GetLevelFactory();

		if (levelFactory != nullptr)
		{
			levelFactory->ChangeLevel(LevelFactory::LevelPaths::Level1);
		}
	}
	else if (buttons[selectedButtonIdx] == EMenuButtons::Controls)
	{
		// TODO: Display controls screen
	}
	else
	{
		// TODO: Exit Game
	}
}