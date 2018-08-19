#pragma once
#include "Level.h"

class MainMenuLevel :
	public Level
{
public:
	MainMenuLevel(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
	virtual ~MainMenuLevel();

	void PrepShaders(std::vector<std::shared_ptr<GraphicsComponent>> graphicsComponentPtrVec, Vector2D<float> cameraPos) override;
};

