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

Overworld1::Overworld1(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader)
    : LevelWithLightingLight(levelWidth, levelHeight, tileVec, vertexShader, fragmentShader)
{
}

Overworld1::~Overworld1()
{
}

void Overworld1::SetupLevel()
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

    bool hasFirstKey = std::dynamic_pointer_cast<PlayerGameStateComponent>(playerPtr->GetGameStateCompPtr())->InventoryContainsItem("FirstKey");
    if(hasFirstKey)
    {
        actorFactoryPtr->KillAllActorsByName("FirstLock");
    }

    bool hasSecondKey = std::dynamic_pointer_cast<PlayerGameStateComponent>(playerPtr->GetGameStateCompPtr())->InventoryContainsItem("SecondKey");
    if (hasSecondKey)
    {
        actorFactoryPtr->KillAllActorsByName("SecondLock");
    }

    if (!(hasFirstKey && hasSecondKey))
    {
        auto door = actorFactoryPtr->GetFirstActorWithName("PortalToBossLevel1");
        door->RemoveLogicCompPtr();
    }
}