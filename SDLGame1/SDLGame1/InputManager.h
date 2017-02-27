#pragma once
#include <cstdint>
class InputManager
{
public:
	InputManager();
	~InputManager();

	uint32_t ReadInput();
};

