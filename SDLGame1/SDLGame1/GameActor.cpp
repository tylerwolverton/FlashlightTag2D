#include "GameActor.h"
#include "InputComponent.h"
#include "Command.h"

GameActor::GameActor()
{
	inputComponent = std::make_shared<InputComponent>();
	commands = std::make_shared<CommandList>();
}


GameActor::~GameActor()
{
}


void GameActor::Update(int deltaMs)
{
	inputComponent->Update(*this, deltaMs);

	for (auto command : *commands)
	{
		if (command)
		{
			command->execute(*this);
		}
	}
}
