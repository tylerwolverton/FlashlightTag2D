#pragma once
#include "Types.h"
//#include "Actor.h"
//class InputComponent;
class Command;

class GameActor //:
	//public Actor
{
public:
	int m_posX, m_posY;

	GameActor(ComponentList components, int posX = 0, int posY = 0);
	virtual ~GameActor();

	virtual void Update(int delatMs);

	virtual void MoveUp() { m_posY -= 10; };
	virtual void MoveDown() { m_posY += 10; };

	std::shared_ptr<CommandList> commands;

private:
	ComponentList m_components;
	//std::shared_ptr<InputComponent> inputComponent;
};

