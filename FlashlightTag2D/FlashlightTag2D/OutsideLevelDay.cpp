#include "OutsideLevelDay.h"
#include "Shader.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"

OutsideLevelDay::OutsideLevelDay(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
    : Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

OutsideLevelDay::~OutsideLevelDay()
{
}

void OutsideLevelDay::PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrVec, Vector2D<float> cameraPos)
{
}