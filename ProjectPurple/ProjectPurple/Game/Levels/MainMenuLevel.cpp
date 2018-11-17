#include "MainMenuLevel.h"

MainMenuLevel::MainMenuLevel(int levelWidth, int levelHeight, 
                             const std::string& spritePath, 
                             const std::string& vertexShader, const std::string& fragmentShader)
    : Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

MainMenuLevel::MainMenuLevel(int levelWidth, int levelHeight,
                             const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr,
                             const std::string& vertexShader, const std::string& fragmentShader)
    : Level(levelWidth, levelHeight, tilePtrVecVecPtr, vertexShader, fragmentShader)
{
}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::PrepShaders(const std::map<ComponentId, std::shared_ptr<GraphicsComponent>>& graphicsComponentPtrMap, 
                                const Vector2D<float>& cameraPos)
{
}