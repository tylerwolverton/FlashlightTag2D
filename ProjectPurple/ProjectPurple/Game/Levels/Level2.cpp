#include "Level2.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"

Level2::Level2(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
    : LevelWithLightingLight(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

Level2::Level2(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader)
    : LevelWithLightingLight(levelWidth, levelHeight, tileVec, vertexShader, fragmentShader)
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

    if(std::dynamic_pointer_cast<PlayerGameStateComponent>(playerPtr->GetGameStateCompPtr())->InventoryContainsItem("SecondKey"))
    {
        actorFactoryPtr->KillAllActorsByName("SecondKey");
    }
}