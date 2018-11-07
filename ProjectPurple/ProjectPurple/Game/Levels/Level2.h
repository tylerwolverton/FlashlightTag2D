#pragma once
#include "LevelWithLightingLight.h"

class Level2 :
    public LevelWithLightingLight
{
public:
    Level2(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    Level2(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader);
    virtual ~Level2();

    virtual void SetupLevel() override;
};

