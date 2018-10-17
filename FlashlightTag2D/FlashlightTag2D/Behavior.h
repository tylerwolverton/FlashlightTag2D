#pragma once
#include "Command.h"

#include <memory>

class Behavior
{
public:
	Behavior();
	virtual ~Behavior();

	virtual std::vector<std::shared_ptr<Command>> Update(const GameActor& actor) { return std::vector<std::shared_ptr<Command>>(); };

protected:
    std::vector<std::shared_ptr<Command>> moveToPosition(Vector2D<float> currentPos,
											             Vector2D<float> targetPos,
														 float speed);
};

