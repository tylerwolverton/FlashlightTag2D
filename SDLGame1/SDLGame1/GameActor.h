#pragma once
#include <memory>
#include <cstdint>
#include "Types.h"
#include "Vector2D.h"

struct SDL_Texture;
class Command;
class World;

class GameActor //:
	//public Actor
{
public:
	Vector2D<int> m_position;
	Vector2D<int> m_size;
	SDL_Texture* m_sprite;
	World*  m_world;
	uint32_t m_input;

	GameActor(World*  world, ComponentList components, Vector2D<int> pos = Vector2D<int>(0, 0), Vector2D<int> size = Vector2D<int>(0, 0), SDL_Texture* sprite = NULL);
	virtual ~GameActor();

	virtual void Update(int delatMs, uint32_t input = 0);

	virtual void MoveUp() { m_position.y -= 10; };
	virtual void MoveDown() { m_position.y += 10; };
	virtual void MoveRight() { m_position.x += 10; };
	virtual void MoveLeft() { m_position.x -= 10; };

	std::shared_ptr<CommandList> commands;

private:
	ComponentList m_components;
};

