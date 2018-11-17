#pragma once
#include "LevelWithLightingLight.h"

class Level2 :
    public LevelWithLightingLight
{
public:
    Level2(int levelWidth, int levelHeight, 
           const std::string& spritePath, 
           const std::string& vertexShader, const std::string& fragmentShader);
    Level2(int levelWidth, int levelHeight, 
           const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr, 
           const std::string& vertexShader, const std::string& fragmentShader);
    virtual ~Level2();

    virtual void SetupLevel() override;
};

