#include "SpawnBehavior.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "ActorFactory.h"
#include "RandomNumberGenerator.h"

#include <SDL.h>

SpawnBehavior::SpawnBehavior(std::vector<int> spawnDelayVec, std::string targetName)
    : m_spawnVecPos(0),
      m_spawnDelayVec(spawnDelayVec),
      m_targetName(targetName)
{
}

SpawnBehavior::~SpawnBehavior()
{
}

std::vector<std::shared_ptr<Command>> SpawnBehavior::Update(const GameActor& thisActor)
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > m_spawnDelayVec[m_spawnVecPos])
    {
        m_lastTickVal = curTicks;
        m_spawnVecPos = (m_spawnVecPos++) % m_spawnDelayVec.size();

        auto transformComp = thisActor.GetTransformComponent();
        if (transformComp != nullptr)
        {
            auto rndNumGen = ServiceLocator::GetRandomNumberGenerator();
            if (rndNumGen != nullptr)
            {
				auto actorFactory = ServiceLocator::GetActorFactory();
				actorFactory->CreateActorFromName(m_targetName, getNextSpawnPos(transformComp->GetPosition()));
            }
        }
    }

    return std::vector<std::shared_ptr<Command>>();
}

Vector2D<float> SpawnBehavior::getNextSpawnPos(Vector2D<float> spawnerPos)
{
    auto rndNumGen = ServiceLocator::GetRandomNumberGenerator();
    if (rndNumGen != nullptr)
    {
        return Vector2D<float>(rndNumGen->GetIntWithinRange(spawnerPos.x - 100, spawnerPos.x + 100),
                               rndNumGen->GetIntWithinRange(spawnerPos.y - 100, spawnerPos.y + 100));
    }
}