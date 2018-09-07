#pragma once
#include <cstdint>
class InputManager
{
public:
	InputManager();
	~InputManager();

	const uint32_t ReadInput() const;

private:
	uint32_t lastTickCount;
};

