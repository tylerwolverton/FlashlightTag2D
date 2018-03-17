#pragma once
#include "Command.h"

class Behavior
{
public:
	Behavior();
	virtual ~Behavior();

	virtual CommandList Update(const GameActor& actor) { return CommandList(); };
};

