#include <SDL.h>

#include "InputManager.h"

InputManager::InputManager()
    : lastTickCount(0)
{
}

InputManager::~InputManager()
{
}

const InputData InputManager::ReadInput() const
{
    InputData input;
    input.buttonsPressed = 0;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W])
    {
        input.buttonsPressed |= EInputValues::W;
    }
    if (keys[SDL_SCANCODE_A])
    {
        input.buttonsPressed |= EInputValues::A;
    }
    if (keys[SDL_SCANCODE_S])
    {
        input.buttonsPressed |= EInputValues::S;
    }
    if (keys[SDL_SCANCODE_D])
    {
        input.buttonsPressed |= EInputValues::D;
    }
    if (keys[SDL_SCANCODE_UP])
    {
        input.buttonsPressed |= EInputValues::Up;
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        input.buttonsPressed |= EInputValues::Left;
    }
    if (keys[SDL_SCANCODE_DOWN])
    {
        input.buttonsPressed |= EInputValues::Down;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        input.buttonsPressed |= EInputValues::Right;
    }
    if (keys[SDL_SCANCODE_SPACE])
    {
        input.buttonsPressed |= EInputValues::Space;
    }
    if (keys[SDL_SCANCODE_RETURN])
    {
        input.buttonsPressed |= EInputValues::Return;
    }

    int x, y;
    const Uint8 mouseState = SDL_GetMouseState(&x, &y);
    input.mousePos = Vector2D<int>(x, y);
    if (mouseState & SDL_BUTTON_LEFT)
    {
        input.buttonsPressed |= EInputValues::MouseLeft;
    }
    if (mouseState & SDL_BUTTON_RIGHT)
    {
        input.buttonsPressed |= EInputValues::MouseRight;
    }
    if (mouseState & SDL_BUTTON_MIDDLE)
    {
        input.buttonsPressed |= EInputValues::MouseMiddle;
    }

    //Handle events on queue
    // SDL_PollEvent takes the next event from the queue, returns 0 if no events are present
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE])
        {
            input.buttonsPressed |= EInputValues::Esc;
        }
    }

    return input;
}