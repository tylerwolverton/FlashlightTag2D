#pragma once
#include "Level.h"

class Level1 :
    public Level
{
public:
    Level1(int levelWidth, int levelHeight,
           const std::string& spritePath,
           const std::string& vertexShader, const std::string& fragmentShader);
    Level1(int levelWidth, int levelHeight,
           const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr,
           const std::string& vertexShader, const std::string& fragmentShader);
    virtual ~Level1();

    void PrepShaders(const std::map<ComponentId, std::shared_ptr<GraphicsComponent>>& graphicsComponentPtrMap,
                     const Vector2D<float>& cameraPos) override;
    virtual void SetupLevel() override;
};

