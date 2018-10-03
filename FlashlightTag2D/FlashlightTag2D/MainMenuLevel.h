#pragma once
#include "Level.h"

class MainMenuLevel :
	public Level
{
public:
	MainMenuLevel(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    MainMenuLevel(int levelWidth, int levelHeight, std::vector<std::vector<int>> tileVec, std::string vertexShader, std::string fragmentShader);
	virtual ~MainMenuLevel();

	void PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos) override;
};

