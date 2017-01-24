#pragma once
#include "Types.h"
//#include "Actor.h"
class InputComponent;
class Command;

class GameActor //:
	//public Actor
{
public:
	int posX, posY;

	GameActor();
	virtual ~GameActor();

	virtual void Update(int delatMs);

	virtual void MoveUp() { posY -= 10; };
	virtual void MoveDown() { posY += 10; };

	std::shared_ptr<CommandList> commands;

private:
	std::shared_ptr<InputComponent> inputComponent;
};

