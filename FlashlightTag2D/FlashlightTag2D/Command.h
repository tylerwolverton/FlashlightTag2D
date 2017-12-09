#pragma once
#include "GameActor.h"
#include "BaseLogicComponent.h"

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
		auto behaviorComponent = actor.GetBaseLogicComponent();
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
        auto behaviorComponent = actor.GetBaseLogicComponent();
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
        auto behaviorComponent = actor.GetBaseLogicComponent();
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
        auto behaviorComponent = actor.GetBaseLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->MoveLeft();
		}
	};
};