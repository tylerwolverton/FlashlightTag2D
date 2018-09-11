#pragma once
#include "GameActor.h"
#include "LogicComponent.h"
#include "Vector2D.h"

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

class Select : public Command
{
public:
	virtual ~Select() {}
	virtual void Execute(GameActor& actor)
	{
		auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->Select();
		}
	};
};

class UpdateMousePosition : public Command
{
public:
	virtual ~UpdateMousePosition() {}
	void SetMousePosition(Vector2D<int> mousePos) { m_mousePos = mousePos; }
	virtual void Execute(GameActor& actor)
	{
		auto behaviorComponent = actor.GetLogicComponent();
		if (behaviorComponent != nullptr)
		{
			behaviorComponent->UpdateMousePosition(m_mousePos);
		}
	};

private:
	Vector2D<int> m_mousePos;
};