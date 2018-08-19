#include "MainMenuLevel.h"

MainMenuLevel::MainMenuLevel(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
	: Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::PrepShaders(std::vector<std::shared_ptr<GraphicsComponent>> graphicsComponentPtrVec, Vector2D<float> cameraPos)
{
}