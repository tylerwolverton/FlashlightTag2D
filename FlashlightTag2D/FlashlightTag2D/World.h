#pragma once
#include "Vector2D.h"
#include "document.h"
//#include "Types.h"

#include <memory>
#include <string>

class InputManager;
class PhysicsManager;
class GraphicsManager;
class ActorFactory;
class Level;

struct SDL_Window;

class World
{
public:
	//Screen dimension constants
	static const int SCREEN_WIDTH = 1920;
	static const int SCREEN_HEIGHT = 1080;

	World(SDL_Window* window);

	virtual ~World();

	void RunGame();
    void ChangeLevel(const std::string& levelPath);

private:
	SDL_Window* m_window;
	std::shared_ptr<Level> m_curLevel;

    std::shared_ptr<InputManager> m_pInputManager;
    std::shared_ptr<PhysicsManager> m_pPhysicsManager;
    std::shared_ptr<GraphicsManager> m_pGraphicsManager;
    std::shared_ptr<ActorFactory> m_pActorFactory;
	
	void changeGameMode();
	std::shared_ptr<Level> createLevelFromJson(const rapidjson::Value& level);
};

