#pragma once
#include "Types.h"
#include "Vector2D.h"

struct SDL_Texture;
//#include "Actor.h"
//class InputComponent;
class Command;

class GameActor //:
	//public Actor
{
public:
	Vector2D<int> m_position;
	Vector2D<int> m_size;
	SDL_Texture* m_sprite;

	GameActor(ComponentList components, Vector2D<int> pos = Vector2D<int>(0, 0), Vector2D<int> size = Vector2D<int>(0, 0), SDL_Texture* sprite = NULL);
	virtual ~GameActor();

	virtual void Update(int delatMs);

	virtual void MoveUp() { m_position.y -= 10; };
	virtual void MoveDown() { m_position.y += 10; };
	virtual void MoveRight() { m_position.x += 10; };
	virtual void MoveLeft() { m_position.x -= 10; };

	std::shared_ptr<CommandList> commands;

private:
	ComponentList m_components;
	//std::shared_ptr<InputComponent> inputComponent;
};

