#include <SDL.h>

#include "InputManager.h"
#include "Types.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

uint32_t InputManager::ReadInput() const
{
	uint32_t input = 0;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W])
	{
		input |= EInputValues::W;
	}
	if (keys[SDL_SCANCODE_A])
	{
		input |= EInputValues::A;
	}
	if (keys[SDL_SCANCODE_S])
	{
		input |= EInputValues::S;
	}
	if (keys[SDL_SCANCODE_D])
	{
		input |= EInputValues::D;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		input |= EInputValues::Space;
	}
	if (keys[SDL_SCANCODE_RETURN])
	{
		input |= EInputValues::Return;
	}

	//Handle events on queue
	// SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE])
		{
			input |= EInputValues::Esc;
		}
	}

	return input;
}