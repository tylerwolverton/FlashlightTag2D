#pragma once
#include "Level.h"

class LevelWithLightingLight :
    public Level
{
public:
    LevelWithLightingLight(int levelWidth, int levelHeight,
                           const std::string& spritePath, 
                           const std::string& vertexShader, const std::string& fragmentShader);
    LevelWithLightingLight(int levelWidth, int levelHeight,
                           const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr, 
                           const std::string& vertexShader, const std::string& fragmentShader);virtual ~LevelWithLightingLight();

    void PrepShaders(const std::map<ComponentId, std::shared_ptr<GraphicsComponent>>& graphicsComponentPtrMap,
                     const Vector2D<float>& cameraPos) override;
};

