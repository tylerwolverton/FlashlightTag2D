#pragma once
#include <cstdint>
#include "Types.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	const InputData ReadInput() const;

private:
	uint32_t lastTickCount;
};

