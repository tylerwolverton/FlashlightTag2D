#pragma once
#include "Behavior.h"
#include "Vector2D.h"

#include <functional>

class SpawnBehavior :
    public Behavior
{
public:
    SpawnBehavior(int spawnDelay, std::string targetName);
    virtual ~SpawnBehavior();

    std::vector<std::shared_ptr<Command>> Update(const GameActor& thisActor) override;

private:
    Vector2D<float> getNextSpawnPos(Vector2D<float> spawnerPos);

    uint32_t m_lastTickVal;
    int m_spawnVecPos;
    int m_spawnDelay;
    //std::function<std::shared_ptr<GameActor>(const ActorFactory&, Vector2D<float>)> m_spawnFunc;
    std::string m_targetName;
};

