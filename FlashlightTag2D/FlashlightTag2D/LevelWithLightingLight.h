#pragma once
#include "Level.h"

class LevelWithLightingLight :
    public Level
{
public:
    LevelWithLightingLight(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    LevelWithLightingLight(int levelWidth, int levelHeight, std::vector<std::vector<int>> tileVec, std::string vertexShader, std::string fragmentShader);
    virtual ~LevelWithLightingLight();

    void PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos) override;
};

