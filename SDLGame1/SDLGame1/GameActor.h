#pragma once
#include "Types.h"
//#include "Actor.h"
//class InputComponent;
class Command;

class GameActor //:
	//public Actor
{
public:
	int posX = 0, posY = 0;

	GameActor(ComponentList components);
	virtual ~GameActor();

	virtual void Update(int delatMs);

	virtual void MoveUp() { posY -= 10; };
	virtual void MoveDown() { posY += 10; };

	std::shared_ptr<CommandList> commands;

private:
	ComponentList m_components;
	//std::shared_ptr<InputComponent> inputComponent;
};

