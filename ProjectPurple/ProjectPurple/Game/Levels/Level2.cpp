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
    std::shared_ptr<ActorFactory> actorFactory = ServiceLocator::GetActorFactory();
    if (actorFactory == nullptr)
    {
        return;
    }

    std::shared_ptr<GameActor> player = actorFactory->GetPlayer();
    if (player == nullptr)
    {
        return;
    }

    if(std::dynamic_pointer_cast<PlayerGameStateComponent>(player->GetGameStateComponent())->InventoryContainsItem("SecondKey"))
    {
        actorFactory->KillAllActorsByName("SecondKey");
    }
}