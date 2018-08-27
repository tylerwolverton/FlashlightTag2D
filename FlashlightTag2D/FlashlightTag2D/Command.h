#pragma once
#include "GameActor.h"
#include "LogicComponent.h"

class Command
{
public:
	virtual ~Command() {}
	virtual void Execute(GameActor& actor) = 0;
};

class MoveUp : public Command
{
public:
	virtual ~MoveUp() {}
	virtual void Execute(GameActor& actor)
	{
		auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->MoveUp();
		}
	};
};

class MoveDown : public Command
{
public:
	virtual ~MoveDown() {}
	virtual void Execute(GameActor& actor)
	{
        auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->MoveDown();
		}
	};
};

class MoveRight : public Command
{
public:
	virtual ~MoveRight() {}
	virtual void Execute(GameActor& actor)
	{
        auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->MoveRight();
		}
	};
};

class MoveLeft : public Command
{
public:
	virtual ~MoveLeft() {}
	virtual void Execute(GameActor& actor)
	{
        auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->MoveLeft();
		}
	};
};

class StartGame : public Command
{
public:
	virtual ~StartGame() {}
	virtual void Execute(GameActor& actor)
	{
		auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->StartGame();
		}
	};
};