#pragma once
#include <memory>
#include <cstdint>
#include "Types.h"
#include "Vector2D.h"

struct SDL_Texture;
class Command;

class GameActor 
{
public:
	GameActor(ComponentList components);
	virtual ~GameActor();

	virtual void Update(int delatMs, uint32_t input = 0);
	StrongActorComponentPtr GetComponentByName(EComponentNames componentName);

	/*virtual void MoveUp() { m_position.y -= 10; };
	virtual void MoveDown() { m_position.y += 10; };
	virtual void MoveRight() { m_position.x += 10; };
	virtual void MoveLeft() { m_position.x -= 10; };*/

	uint32_t GetInput() { return m_input; }
	void SetInput(uint32_t input) { m_input = input; }

	std::shared_ptr<CommandList> GetCommands() { return m_commands; }
	void SetCommands(std::shared_ptr<CommandList> commands) { m_commands = commands; }

private:
	ComponentList m_components;
	uint32_t m_input;
	std::shared_ptr<CommandList> m_commands;
};

