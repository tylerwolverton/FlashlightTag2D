#include "Level1.h"
#include "Shader.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"

#include <SDL.h>

#define MAX_NUM_LIGHTS 10

Level1::Level1(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
    : Level(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

Level1::Level1(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader)
    : Level(levelWidth, levelHeight, tileVec, vertexShader, fragmentShader)
{
}

Level1::~Level1()
{
}

void Level1::PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsCompPtrMap, Vector2D<float> cameraPos)
{
    // opengl likes flat arrays...
    int lightCount = 0;
    int flashingLightCount = 0;
    std::vector<GLfloat> lightSrcVec;
    std::vector<GLfloat> lightDirVec;
    //std::vector<GLfloat> lightPosVec;
    std::vector<GLfloat> flashingLightSrcVec;
    std::vector<GLfloat> flashingLightColorVec;
    for (auto graphicsCompPtrEntry : graphicsCompPtrMap)
    {
        if (lightCount >= MAX_NUM_LIGHTS) { break; }

        auto actorTransformCompPtr = *(graphicsCompPtrEntry.second->GetTransformCompPtr());

        auto parentActorPtr = graphicsCompPtrEntry.second->GetParent();
        if (parentActorPtr == nullptr)
        {
            continue;
        }

        auto gameStateCompPtr = parentActorPtr->GetGameStateCompPtr();
        if (gameStateCompPtr == nullptr)
        {
            continue;
        }

        // Static lights
        if(gameStateCompPtr->GetName() == "Player")
        {
            if (lightCount >= MAX_NUM_LIGHTS) { break; }

            Vector2D<float> actorPos = actorTransformCompPtr.GetPosition();
            Vector2D<float> actorSize = actorTransformCompPtr.GetSize();
            Vector2D<float> actorLocation = actorPos - cameraPos - graphicsCompPtrEntry.second->GetImageOffset();

            lightSrcVec.push_back(actorLocation.x + actorSize.x / 2); 
            lightSrcVec.push_back(actorLocation.y + actorSize.y / 2); 
            lightSrcVec.push_back(100.0f);

            lightDirVec.push_back(actorTransformCompPtr.GetDirection().x);
            lightDirVec.push_back(actorTransformCompPtr.GetDirection().y);

            lightCount++;
        }
        // Flashing lights
        else if (gameStateCompPtr->GetName() == "Projectile")
        {
            if (flashingLightCount >= MAX_NUM_LIGHTS) { break; }

            Vector2D<float> actorPos = actorTransformCompPtr.GetPosition();
            Vector2D<float> actorSize = actorTransformCompPtr.GetSize();
            Vector2D<float> actorLocation = actorPos - cameraPos - graphicsCompPtrEntry.second->GetImageOffset();

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
    if (lightCount < MAX_NUM_LIGHTS)
    {
        lightSrcVec.push_back(-901.0f); 
        lightSrcVec.push_back(-901.0f); 
        lightSrcVec.push_back(-901.0f);
    }
    if (flashingLightCount < MAX_NUM_LIGHTS)
    {
        flashingLightSrcVec.push_back(-901.0f); 
        flashingLightSrcVec.push_back(-901.0f); 
        flashingLightSrcVec.push_back(-901.0f);
    }

    m_shaderPtr->SetInt("timeElapsed", SDL_GetTicks()/50);
    m_shaderPtr->SetVec3("lightSrc", &lightSrcVec.front(), lightSrcVec.size() / 3);
    m_shaderPtr->SetVec3("flashingLightSrc", &flashingLightSrcVec.front(), flashingLightSrcVec.size() / 3);
    if (flashingLightCount > 0)
    {
        m_shaderPtr->SetVec3("flashingLightColor", &flashingLightColorVec.front(), flashingLightColorVec.size() / 3);
    }
}

void Level1::SetupLevel() 
{
    auto actorFactoryPtr = ServiceLocator::GetActorFactory();
    if (actorFactoryPtr == nullptr)
    {
        return;
    }

    auto playerPtr = actorFactoryPtr->GetPlayer();
    if (playerPtr == nullptr)
    {
        return;
    }

    if(std::dynamic_pointer_cast<PlayerGameStateComponent>(playerPtr->GetGameStateCompPtr())->InventoryContainsItem("FirstKey"))
    {
        actorFactoryPtr->KillAllActorsByName("FirstKey");
    }
}