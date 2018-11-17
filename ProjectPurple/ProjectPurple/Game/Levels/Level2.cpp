#include "Level2.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"

Level2::Level2(int levelWidth, int levelHeight, 
               const std::string& spritePath, 
               const std::string& vertexShader, const std::string& fragmentShader)
    : LevelWithLightingLight(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

Level2::Level2(int levelWidth, int levelHeight, 
               const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr, 
               const std::string& vertexShader, const std::string& fragmentShader)
    : LevelWithLightingLight(levelWidth, levelHeight, tilePtrVecVecPtr, vertexShader, fragmentShader)
{
}

Level2::~Level2()
{
}

void Level2::SetupLevel()
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

    // Remove key if player already has it
    if(std::dynamic_pointer_cast<PlayerGameStateComponent>(playerPtr->GetGameStateCompPtr())->InventoryContainsItem("SecondKey"))
    {
        actorFactoryPtr->KillAllActorsByName("SecondKey");
    }
}