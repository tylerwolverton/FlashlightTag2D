#include "SpawnBehavior.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"
#include "RandomNumberGenerator.h"

#include <SDL.h>

SpawnBehavior::SpawnBehavior(int spawnDelay, const std::string& targetName)
    : m_spawnVecPos(0),
      m_spawnDelay(spawnDelay),
      m_targetName(targetName)
{
}

SpawnBehavior::~SpawnBehavior()
{
}

std::vector<std::shared_ptr<Command>> SpawnBehavior::Update(const GameActor& thisActor)
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > m_spawnDelay)
    {
        m_lastTickVal = curTicks;

        auto transformCompPtr = thisActor.GetTransformCompPtr();
        if (transformCompPtr != nullptr)
        {
            auto rndNumGen = ServiceLocator::GetRandomNumberGenerator();
            if (rndNumGen != nullptr)
            {
                auto actorFactoryPtr = ServiceLocator::GetActorFactory();
                actorFactoryPtr->CreateActorFromName(m_targetName, getNextSpawnPos(transformCompPtr->GetPosition()));
            }
        }
    }

    return std::vector<std::shared_ptr<Command>>();
}

Vector2D<float> SpawnBehavior::getNextSpawnPos(const Vector2D<float>& spawnerPos)
{
    auto rndNumGenPtr = ServiceLocator::GetRandomNumberGenerator();
    if (rndNumGenPtr != nullptr)
    {
        return Vector2D<float>(rndNumGenPtr->GetFloatWithinRange(spawnerPos.x - 100, spawnerPos.x + 100),
                               rndNumGenPtr->GetFloatWithinRange(spawnerPos.y - 100, spawnerPos.y + 100));
    }

    return Vector2D<float>(0,0);
}