#pragma once
#include "Command.h"

class Behavior
{
public:
	Behavior();
	virtual ~Behavior();

	virtual CommandList Update() = 0;
};

