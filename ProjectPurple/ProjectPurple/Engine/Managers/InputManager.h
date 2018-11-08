#pragma once
#include <cstdint>
#include "Types.h"

class InputManager
{
public:
    InputManager();
    ~InputManager();

    const InputData ReadInput() const;
};

