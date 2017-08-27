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

	uint32_t GetInput() { return m_input; }
	void SetInput(uint32_t input) { m_input = input; }

	std::shared_ptr<CommandList> GetCommands() { return m_pCommands; }
	void SetCommands(std::shared_ptr<CommandList> commands) { m_pCommands = commands; }

private:
	ComponentList m_components;
	uint32_t m_input;
	std::shared_ptr<CommandList> m_pCommands;
};

