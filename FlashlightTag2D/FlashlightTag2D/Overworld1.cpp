#include "Overworld1.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"
#include "GameStateComponent.h"
#include "PlayerGameStateComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"

Overworld1::Overworld1(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
	: LevelWithLightingLight(levelWidth, levelHeight, spritePath, vertexShader, fragmentShader)
{
}

Overworld1::Overworld1(int levelWidth, int levelHeight, std::vector<std::vector<int>> tileVec, std::string vertexShader, std::string fragmentShader)
    : LevelWithLightingLight(levelWidth, levelHeight, tileVec, vertexShader, fragmentShader)
{
}

Overworld1::~Overworld1()
{
}

void Overworld1::SetupLevel()
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

    bool hasFirstKey = std::dynamic_pointer_cast<PlayerGameStateComponent>(player->GetGameStateComponent())->InventoryContainsItem("FirstKey");
    if(hasFirstKey)
    {
        actorFactory->KillAllActorsByName("FirstLock");
    }

    bool hasSecondKey = std::dynamic_pointer_cast<PlayerGameStateComponent>(player->GetGameStateComponent())->InventoryContainsItem("SecondKey");
    if (hasSecondKey)
    {
        actorFactory->KillAllActorsByName("SecondLock");
    }

    if (!(hasFirstKey && hasSecondKey))
    {
        auto door = actorFactory->GetFirstActorWithName("PortalToBossLevel1");
        door->RemoveLogicComponent();
    }
}