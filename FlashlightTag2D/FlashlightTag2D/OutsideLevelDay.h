#pragma once
#include "Level.h"

class OutsideLevelDay :
    public Level
{
public:
    OutsideLevelDay(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    virtual ~OutsideLevelDay();

    void PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos) override;
};

