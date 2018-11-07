#pragma once
#include "Level.h"

class Level1 :
    public Level
{
public:
    Level1(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    Level1(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader);
    virtual ~Level1();

    void PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos) override;
    virtual void SetupLevel() override;
};

