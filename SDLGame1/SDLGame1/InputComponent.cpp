#include "InputComponent.h"
#include <SDL.h>

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
	/*bool handleInput(uint32_t& buttonState)
	{*/
	uint32_t buttonState = 0;
		//Handle events on queue
		// SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				buttonState |= EInputValues::Esc;
			}

			const Uint8 *keys = SDL_GetKeyboardState(NULL);

			if (keys[SDL_SCANCODE_W])
			{
				buttonState |= EInputValues::W;
			}
			else
			{
				buttonState &= ~(EInputValues::W);
			}
			if (keys[SDL_SCANCODE_A])
			{
				buttonState |= EInputValues::A;
			}
			else
			{
				buttonState &= ~(EInputValues::A);
			}
			if (keys[SDL_SCANCODE_S])
			{
				buttonState |= EInputValues::S;
			}
			else
			{
				buttonState &= ~(EInputValues::S);
			}
			if (keys[SDL_SCANCODE_D])
			{
				buttonState |= EInputValues::D;
			}
			else
			{
				buttonState &= ~(EInputValues::D);
			}
			if (keys[SDL_SCANCODE_ESCAPE])
			{
				buttonState |= EInputValues::Esc;
			}
		//}

}

ComponentId InputComponent::VGetComponentId() const
{
	return ComponentId();
}