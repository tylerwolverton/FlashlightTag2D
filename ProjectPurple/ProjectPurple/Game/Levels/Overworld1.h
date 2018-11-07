#pragma once
#include "LevelWithLightingLight.h"

class Overworld1 :
    public LevelWithLightingLight
{
public:
    Overworld1(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    Overworld1(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader);
    virtual ~Overworld1();

    virtual void SetupLevel() override;
};

