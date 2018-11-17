#include "LevelWithLightingLight.h"
#include "Shader.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"
#include "GameStateComponent.h"

#include <SDL.h>

#define MAX_NUM_LIGHTS 30

LevelWithLightingLight::LevelWithLightingLight(int levelWidth, int levelHeight,
                                               const std::string& spritePath,
                                               const std::string& vertexShader, const std::string& fragmentShader) 
    : Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

LevelWithLightingLight::LevelWithLightingLight(int levelWidth, int levelHeight, 
                                               const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr, 
                                               const std::string& vertexShader, const std::string& fragmentShader)
    : Level(levelWidth, levelHeight, tilePtrVecVecPtr, vertexShader, fragmentShader)
{
}

LevelWithLightingLight::~LevelWithLightingLight()
{
}

void LevelWithLightingLight::PrepShaders(const std::map<ComponentId, std::shared_ptr<GraphicsComponent>>& graphicsComponentPtrVec, 
                                         const Vector2D<float>& cameraPos)
{
    // opengl likes flat arrays...
    int flashingLightCount = 0;
    std::vector<GLfloat> flashingLightSrcVec;
    std::vector<GLfloat> flashingLightColorVec;
    for (auto graphicsComponent : graphicsComponentPtrVec)
    {
        auto actorTransformCompPtr = *(graphicsComponent.second->GetTransformCompPtr());

        auto parentActorPtr = graphicsComponent.second->GetParent();
        if (parentActorPtr == nullptr)
        {
            continue;
        }

        auto gameStateCompPtr = parentActorPtr->GetGameStateCompPtr();
        if (gameStateCompPtr == nullptr)
        {
            continue;
        }

        // Flashing lights
        else if (gameStateCompPtr->GetName() == "Projectile")
        {
            if (flashingLightCount >= MAX_NUM_LIGHTS) { break; }

            Vector2D<float> actorPos = actorTransformCompPtr.GetPosition();
            Vector2D<float> actorSize = actorTransformCompPtr.GetSize();
            Vector2D<float> actorLocation = actorPos - cameraPos - graphicsComponent.second->GetImageOffset();

            flashingLightSrcVec.push_back(actorLocation.x + actorSize.x / 2);
            flashingLightSrcVec.push_back(actorLocation.y + actorSize.y / 2);
            flashingLightSrcVec.push_back(actorSize.x + 10.0f);

            flashingLightColorVec.push_back(1);
            flashingLightColorVec.push_back(0);
            flashingLightColorVec.push_back(0);

            flashingLightCount++;
        }
    }

    // Use a special vector to tell the shader there are no more lights
    if (flashingLightCount < MAX_NUM_LIGHTS)
    {
        flashingLightSrcVec.push_back(-901.0f);
        flashingLightSrcVec.push_back(-901.0f);
        flashingLightSrcVec.push_back(-901.0f);
    }

    m_shaderPtr->SetInt("timeElapsed", SDL_GetTicks() / 50);
    m_shaderPtr->SetVec3("flashingLightSrc", &flashingLightSrcVec.front(), flashingLightSrcVec.size() / 3);
    if (flashingLightCount > 0)
    {
        m_shaderPtr->SetVec3("flashingLightColor", &flashingLightColorVec.front(), flashingLightColorVec.size() / 3);
    }
}