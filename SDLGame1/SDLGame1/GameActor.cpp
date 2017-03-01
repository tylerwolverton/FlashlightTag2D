#include <SDL.h>
#include "GameActor.h"
#include "ActorComponent.h"
#include "Command.h"

GameActor::GameActor(World*  world, ComponentList components, Vector2D<int> pos, Vector2D<int> size, SDL_Texture* sprite)
	: m_world(world),
	  m_components(components),
	  m_position(pos),
	  m_size(size),
	  m_sprite(sprite)
{
	commands = std::make_shared<CommandList>();
}

GameActor::~GameActor()
{
	SDL_DestroyTexture(m_sprite);
	m_sprite = NULL;
}

void GameActor::Update(int deltaMs, uint32_t input)
{
	m_input = input;

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

StrongActorComponentPtr GameActor::GetComponentByName(EComponentNames componentName)
{
	for (auto component : m_components)
	{
		if (component->GetComponentName() == componentName)
		{
			return component;
		}
	}

	return nullptr;
}
