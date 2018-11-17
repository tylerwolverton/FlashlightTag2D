#pragma once
#include "LevelWithLightingLight.h"

class Overworld1 :
    public LevelWithLightingLight
{
public:
    Overworld1(int levelWidth, int levelHeight,
               const std::string& spritePath,
               const std::string& vertexShader, const std::string& fragmentShader);
    Overworld1(int levelWidth, int levelHeight,
               const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr,
               const std::string& vertexShader, const std::string& fragmentShader); 
    virtual ~Overworld1();

    virtual void SetupLevel() override;
};

