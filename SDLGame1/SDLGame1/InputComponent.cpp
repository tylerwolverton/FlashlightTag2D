#include "InputComponent.h"
#include "GameActor.h"
#include "Command.h"
#include <SDL.h>
#include <list>

InputComponent::InputComponent()
{
	buttonW = std::make_shared<MoveUp>();
	buttonS = std::make_shared<MoveDown>();
}


InputComponent::~InputComponent()
{
}

bool InputComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
}

void InputComponent::PostInit()
{
}

void InputComponent::Update(GameActor& actor, int deltaMs)
{
	actor.commands = std::make_shared<CommandList>(HandleInput());
}

CommandList InputComponent::HandleInput()
{
	CommandList commandList;
	//uint32_t buttonState = 0;
	//Handle events on queue
	// SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			commandList.push_back(buttonEsc);
			//buttonState |= EInputValues::Esc;
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_W])
		{
			commandList.push_back(buttonW);
			//buttonState |= EInputValues::W;
		}
		else
		{
			//buttonState &= ~(EInputValues::W);
		}
		if (keys[SDL_SCANCODE_A])
		{
			commandList.push_back(buttonA);
			//buttonState |= EInputValues::A;
		}
		else
		{
			//buttonState &= ~(EInputValues::A);
		}
		if (keys[SDL_SCANCODE_S])
		{
			commandList.push_back(buttonS);
			//buttonState |= EInputValues::S;
		}
		else
		{
			//buttonState &= ~(EInputValues::S);
		}
		if (keys[SDL_SCANCODE_D])
		{
			commandList.push_back(buttonD);
			//buttonState |= EInputValues::D;
		}
		else
		{
			//buttonState &= ~(EInputValues::D);
		}
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			commandList.push_back(buttonEsc);
			//buttonState |= EInputValues::Esc;
		}
	}

	return commandList;
}

ComponentId InputComponent::GetComponentId() const
{
	return ComponentId();
}