#include "MainMenuLogicComponent.h"
#include "ServiceLocator.h"
#include "LevelFactory.h"

MainMenuLogicComponent::MainMenuLogicComponent(ComponentId componentId)
	: LogicComponent(componentId, nullptr)
{
}

MainMenuLogicComponent::~MainMenuLogicComponent()
{
}

void MainMenuLogicComponent::MoveUp()
{

}

void MainMenuLogicComponent::MoveDown()
{

}

void MainMenuLogicComponent::StartGame()
{
	std::shared_ptr<LevelFactory> levelFactory = ServiceLocator::GetLevelFactory();

	if (levelFactory != nullptr)
	{
		levelFactory->ChangeLevel(LevelFactory::LevelPaths::Level1);
	}
}