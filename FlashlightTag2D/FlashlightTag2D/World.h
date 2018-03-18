#pragma once
#include "Vector2D.h"
#include "Types.h"

#include <memory>

// Move to graphics manager
#define MAX_CAMERAS 10
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
    Vector2D<int> m_levelSize;

    std::unique_ptr<InputManager> m_pInputManager;
    std::unique_ptr<PhysicsManager> m_pPhysicsManager;
    std::unique_ptr<GraphicsManager> m_pGraphicsManager;
    std::shared_ptr<ActorFactory> m_pActorFactory;

	void changeGameMode();
};

