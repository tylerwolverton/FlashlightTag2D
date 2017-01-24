#pragma once
#include "GameActor.h"

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(GameActor& actor) = 0;
};

class MoveUp : public Command
{
public:
	virtual ~MoveUp() {}
	virtual void execute(GameActor& actor) { actor.MoveUp(); };
};

class MoveDown : public Command
{
public:
	virtual ~MoveDown() {}
	virtual void execute(GameActor& actor) { actor.MoveDown(); };
};