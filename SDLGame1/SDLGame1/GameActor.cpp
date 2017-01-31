#include <SDL.h>
#include "GameActor.h"
#include "ActorComponent.h"
#include "Command.h"

GameActor::GameActor(ComponentList components, int posX, int posY, int size, SDL_Texture* sprite)
	: m_components(components),
	  m_posX(posX),
	  m_posY(posY),
	  m_size(size),
	  m_sprite(sprite)
{
	//inputComponent = std::make_shared<InputComponent>();
	commands = std::make_shared<CommandList>();
}


GameActor::~GameActor()
{
	SDL_DestroyTexture(m_sprite);
	m_sprite = NULL;
}


void GameActor::Update(int deltaMs)
{
	//inputComponent->Update(*this, deltaMs);
	for (auto comp : m_components)
	{
		comp->Update(*this, deltaMs);

		for (auto command : *commands)
		{
			if (command)
			{
				command->execute(*this);
			}
		}
		commands->clear();
	}
}
