#include "MainMenuLevel.h"

MainMenuLevel::MainMenuLevel(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
	: Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

MainMenuLevel::MainMenuLevel(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader)
    : Level(levelWidth, levelHeight, tileVec, vertexShader, fragmentShader)
{
}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos)
{
}