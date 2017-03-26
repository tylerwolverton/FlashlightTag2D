#pragma once
#include <memory>
#include <cstdint>
#include "Types.h"
#include "Vector2D.h"

struct SDL_Texture;
class Command;

class GameActor //:
	//public Actor
{
public:
	Vector2D<int> m_position;
	Vector2D<int> m_size;
	uint32_t m_input;

	GameActor(ComponentList components, Vector2D<int> pos = Vector2D<int>(0, 0), Vector2D<int> size = Vector2D<int>(0, 0));
	virtual ~GameActor();

	virtual void Update(int delatMs, uint32_t input = 0);
	StrongActorComponentPtr GetComponentByName(EComponentNames componentName);

	virtual void MoveUp() { m_position.y -= 10; };
	virtual void MoveDown() { m_position.y += 10; };
	virtual void MoveRight() { m_position.x += 10; };
	virtual void MoveLeft() { m_position.x -= 10; };

	std::shared_ptr<CommandList> commands;

private:
	ComponentList m_components;
};

