#include "InputComponent.h"
#include <SDL.h>
#include <list>

InputComponent::InputComponent()
{
}


InputComponent::~InputComponent()
{
}

bool InputComponent::VInit(tinyxml2::XMLElement* pData)
{
	return true;
}

void InputComponent::VPostInit()
{
}

void InputComponent::VUpdate(int deltaMs)
{
	

}

std::list<std::shared_ptr<Command>> InputComponent::HandleInput()
{
	std::list<std::shared_ptr<Command>> commandList;
	uint32_t buttonState = 0;
	//Handle events on queue
	// SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			commandList.push_back(buttonEsc);
			buttonState |= EInputValues::Esc;
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_W])
		{
			commandList.push_back(buttonW);
			buttonState |= EInputValues::W;
		}
		else
		{
			buttonState &= ~(EInputValues::W);
		}
		if (keys[SDL_SCANCODE_A])
		{
			commandList.push_back(buttonA);
			buttonState |= EInputValues::A;
		}
		else
		{
			buttonState &= ~(EInputValues::A);
		}
		if (keys[SDL_SCANCODE_S])
		{
			commandList.push_back(buttonS);
			buttonState |= EInputValues::S;
		}
		else
		{
			buttonState &= ~(EInputValues::S);
		}
		if (keys[SDL_SCANCODE_D])
		{
			commandList.push_back(buttonD);
			buttonState |= EInputValues::D;
		}
		else
		{
			buttonState &= ~(EInputValues::D);
		}
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			commandList.push_back(buttonEsc);
			buttonState |= EInputValues::Esc;
		}
	}

	return commandList;
}

ComponentId InputComponent::VGetComponentId() const
{
	return ComponentId();
}