#pragma once
#include "Vector2D.h"
#include "Types.h"

#define MAX_CAMERAS 10
struct SDL_Window;

class World
{
public:
    Vector2D<int> GetLevelSize() { return m_levelSize; }

	//Screen dimension constants
	static const int SCREEN_WIDTH = 1920;
	static const int SCREEN_HEIGHT = 1080;

	World(SDL_Window* window);

	virtual ~World();

	void RunGame();
	void AddCamera(StrongGameActorPtr camera);

    void ChangeLevel(std::string levelPath, StrongGraphicsManagerPtr graphicsManager, StrongActorFactoryPtr actorFactory);

	StrongGameActorPtr GetCurrentCamera() { return m_pCurrentCamera; };

private:
	ComponentList m_graphicsComponentList;
	StrongGameActorPtrList m_pCameraList;
	StrongGameActorPtr m_pCurrentCamera;
	SDL_Window* m_window;
    Vector2D<int> m_levelSize;
};

