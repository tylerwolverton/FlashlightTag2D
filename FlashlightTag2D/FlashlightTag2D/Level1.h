#pragma once
#include "Level.h"

class Level1 :
	public Level
{
public:
	Level1(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
	virtual ~Level1();

	void PrepShaders(std::vector<std::shared_ptr<GraphicsComponent>> graphicsComponentPtrVec, Vector2D<float> cameraPos) override;
};
