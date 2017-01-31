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

class MoveRight : public Command
{
public:
	virtual ~MoveRight() {}
	virtual void execute(GameActor& actor) { actor.MoveRight(); };
};

class MoveLeft : public Command
{
public:
	virtual ~MoveLeft() {}
	virtual void execute(GameActor& actor) { actor.MoveLeft(); };
};