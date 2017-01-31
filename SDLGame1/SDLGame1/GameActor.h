#pragma once
#include "Types.h"

struct SDL_Texture;
//#include "Actor.h"
//class InputComponent;
class Command;

class GameActor //:
	//public Actor
{
public:
	int m_posX, m_posY;
	SDL_Texture* m_sprite;

	GameActor(ComponentList components, int posX = 0, int posY = 0, SDL_Texture* sprite = NULL);
	virtual ~GameActor();

	virtual void Update(int delatMs);

	virtual void MoveUp() { m_posY -= 10; };
	virtual void MoveDown() { m_posY += 10; };
	virtual void MoveRight() { m_posX += 10; };
	virtual void MoveLeft() { m_posX -= 10; };

	std::shared_ptr<CommandList> commands;

private:
	ComponentList m_components;
	//std::shared_ptr<InputComponent> inputComponent;
};

