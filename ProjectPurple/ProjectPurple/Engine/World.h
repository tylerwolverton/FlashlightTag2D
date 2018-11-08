#pragma once
#include <memory>
#include <string>

class InputManager;
class PhysicsManager;
class GraphicsManager;
class ActorFactory;
class LevelFactory;
class Level;

struct SDL_Window;

class World
{
public:
    //Screen dimension constants
    static const int SCREEN_WIDTH = 1280;
    static const int SCREEN_HEIGHT = 720;

    World(SDL_Window* window);

    virtual ~World();

    void RunGame();
    void QuitGame();

private:
    bool m_isGameRunning;
    float m_escHoldTime;

    std::shared_ptr<InputManager> m_inputManagerPtr;
    std::shared_ptr<PhysicsManager> m_physicsManagerPtr;
    std::shared_ptr<GraphicsManager> m_graphicsManagerPtr;
    std::shared_ptr<ActorFactory> m_actorFactoryPtr;
    std::shared_ptr<LevelFactory> m_levelFactoryPtr;
};

