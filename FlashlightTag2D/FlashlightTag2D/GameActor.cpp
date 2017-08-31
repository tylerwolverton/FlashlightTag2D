#include <SDL.h>
#include "GameActor.h"
#include "ActorComponent.h"
#include "Command.h"

GameActor::GameActor(ComponentList components)
	: m_components(components)
{
	m_pCommands = std::make_shared<CommandList>();
}

GameActor::~GameActor()
{
}

void GameActor::Update(int deltaMs, uint32_t input)
{
	m_input = input;

	for (auto comp : m_components)
	{
		comp->Update(*this, deltaMs);

		for (auto command : *GetCommands())
		{
			if (command)
			{
				command->Execute(*this);
			}
		}
		GetCommands()->clear();
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
