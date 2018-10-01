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
		auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->MoveUp();
		}
	};
};

class MoveDown : public Command
{
public:
	virtual ~MoveDown() {}
	virtual void Execute(GameActor& actor)
	{
        auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->MoveDown();
		}
	};
};

class MoveRight : public Command
{
public:
	virtual ~MoveRight() {}
	virtual void Execute(GameActor& actor)
	{
        auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->MoveRight();
		}
	};
};

class MoveLeft : public Command
{
public:
	virtual ~MoveLeft() {}
	virtual void Execute(GameActor& actor)
	{
        auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->MoveLeft();
		}
	};
};

class Select : public Command
{
public:
	virtual ~Select() {}
	virtual void Execute(GameActor& actor)
	{
		auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->Select();
		}
	};
};

class Shoot : public Command
{
public:
	virtual ~Shoot() {}
	virtual void Execute(GameActor& actor)
	{
		auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->Shoot();
		}
	};
};

class Spawn : public Command
{
public:
	virtual ~Spawn() {}
	void SetSpawnLocation(Vector2D<float> location) { m_location = location; }
	virtual void Execute(GameActor& actor)
	{
		auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->Spawn();
		}
	};
private:
	Vector2D<float> m_location;
};

class UpdateMousePosition : public Command
{
public:
	virtual ~UpdateMousePosition() {}
	void SetMousePosition(Vector2D<int> mousePos) { m_mousePos = mousePos; }
	virtual void Execute(GameActor& actor)
	{
		auto logicComponent = actor.GetLogicComponent();
		if (logicComponent != nullptr)
		{
			logicComponent->UpdateMousePosition(m_mousePos);
		}
	};

private:
	Vector2D<int> m_mousePos;
};