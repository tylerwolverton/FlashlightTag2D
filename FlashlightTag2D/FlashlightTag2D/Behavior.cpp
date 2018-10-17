#include "Behavior.h"

Behavior::Behavior()
{
}

Behavior::~Behavior()
{
}

std::vector<std::shared_ptr<Command>> Behavior::moveToPosition(Vector2D<float> currentPos,
															   Vector2D<float> targetPos, 
															   float speed)
{
	std::vector<std::shared_ptr<Command>> commands;

	Vector2D<float> dist = targetPos - currentPos;

	if (dist.x < -speed)
	{
		commands.push_back(std::make_shared<MoveLeft>());
	}
	else if (dist.x > speed)
	{
		commands.push_back(std::make_shared<MoveRight>());
	}

	if (dist.y < -speed)
	{
		commands.push_back(std::make_shared<MoveDown>());
	}
	else if (dist.y > speed)
	{
		commands.push_back(std::make_shared<MoveUp>());
	}

	return commands;
}