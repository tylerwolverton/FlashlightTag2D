#include <SDL.h>

#include "MainMenuLogicComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "World.h"
#include "LevelFactory.h"

MainMenuLogicComponent::MainMenuLogicComponent(ComponentId componentId, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>> buttonTransformCompMapPtr)
    : LogicComponent(componentId),
      m_buttonTransformCompMapPtr(buttonTransformCompMapPtr),
      selectedButtonIdx(0)
{
    buttons.push_back(EMenuButtons::Play);
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
            m_buttonTransformCompMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompMapPtr->find("Exit")->second->GetPosition());
        }
        else
        {
            m_buttonTransformCompMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompMapPtr->find("Play")->second->GetPosition());
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
            m_buttonTransformCompMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompMapPtr->find("Exit")->second->GetPosition());
        }
        else
        {
            m_buttonTransformCompMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompMapPtr->find("Play")->second->GetPosition());
        }


        selectedButtonIdx++;
        if (selectedButtonIdx >= buttons.size())
        {
            selectedButtonIdx = 0;
        }
    }
}

void MainMenuLogicComponent::Select()
{
    if (buttons[selectedButtonIdx] == EMenuButtons::Play)
    {
        std::shared_ptr<LevelFactory> levelFactory = ServiceLocator::GetLevelFactory();

        if (levelFactory != nullptr)
        {
            levelFactory->ChangeLevel(LevelFactory::LevelPaths::ControlsScreen);
        }
    }
    else
    {
        std::shared_ptr<World> world = ServiceLocator::GetWorld();

        if (world != nullptr)
        {
            world->QuitGame();
        }
    }
}