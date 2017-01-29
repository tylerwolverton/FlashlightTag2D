#include "GameActor.h"
#include "ActorComponent.h"
#include "Command.h"

GameActor::GameActor(ComponentList components, int posX, int posY)
	: m_components(components),
	  m_posX(posX),
	  m_posY(posY)
{
	//inputComponent = std::make_shared<InputComponent>();
	commands = std::make_shared<CommandList>();
}


GameActor::~GameActor()
{
}


void GameActor::Update(int deltaMs)
{
	//inputComponent->Update(*this, deltaMs);
	for (auto comp : m_components)
	{
		comp->Update(*this, deltaMs);
	}

	for (auto command : *commands)
	{
		if (command)
		{
			command->execute(*this);
		}
	}
}
