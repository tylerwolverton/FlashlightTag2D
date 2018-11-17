#include <SDL.h>

#include "MainMenuLogicComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "World.h"
#include "LevelFactory.h"

MainMenuLogicComponent::MainMenuLogicComponent(ComponentId componentId, 
                                               const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>>& buttonTransformCompPtrMapPtr)
    : LogicComponent(componentId),
	  m_buttonTransformCompPtrMapPtr(buttonTransformCompPtrMapPtr),
      m_selectedButtonIdx(0)
{
    m_buttonVec.push_back(EMenuButtons::Play);
    m_buttonVec.push_back(EMenuButtons::Exit);
}

MainMenuLogicComponent::~MainMenuLogicComponent()
{
}

void MainMenuLogicComponent::MoveUp()
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > 200)
    {
        m_lastTickVal = curTicks;
        if (m_buttonVec[m_selectedButtonIdx] == EMenuButtons::Play)
        {
			m_buttonTransformCompPtrMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompPtrMapPtr->find("Exit")->second->GetPosition());
        }
        else
        {
			m_buttonTransformCompPtrMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompPtrMapPtr->find("Play")->second->GetPosition());
        }

        m_selectedButtonIdx--;
        if (m_selectedButtonIdx < 0)
        {
            m_selectedButtonIdx = m_buttonVec.size() - 1;
        }
    }
}

void MainMenuLogicComponent::MoveDown()
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > 200)
    {
        m_lastTickVal = curTicks;
        if (m_buttonVec[m_selectedButtonIdx] == EMenuButtons::Play)
        {
			m_buttonTransformCompPtrMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompPtrMapPtr->find("Exit")->second->GetPosition());
        }
        else
        {
			m_buttonTransformCompPtrMapPtr->find("SelectionBorder")->second->SetPosition(m_buttonTransformCompPtrMapPtr->find("Play")->second->GetPosition());
        }


        m_selectedButtonIdx++;
        if (m_selectedButtonIdx >= m_buttonVec.size())
        {
            m_selectedButtonIdx = 0;
        }
    }
}

void MainMenuLogicComponent::Select()
{
    if (m_buttonVec[m_selectedButtonIdx] == EMenuButtons::Play)
    {
        auto levelFactoryPtr = ServiceLocator::GetLevelFactory();

        if (levelFactoryPtr != nullptr)
        {
            levelFactoryPtr->ChangeLevel(LevelFactory::LevelPaths::ControlsScreen);
        }
    }
    else
    {
        auto worldPtr = ServiceLocator::GetWorld();

        if (worldPtr != nullptr)
        {
            worldPtr->QuitGame();
        }
    }
}