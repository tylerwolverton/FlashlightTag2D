#pragma once
#include "Behavior.h"
#include "Vector2D.h"

#include <functional>

class SpawnBehavior :
    public Behavior
{
public:
    SpawnBehavior(int spawnDelay, const std::string& targetName);
    virtual ~SpawnBehavior();

    std::vector<std::shared_ptr<Command>> Update(const GameActor& thisActor) override;

private:
    uint32_t m_lastTickVal;
    int m_spawnVecPos;
    int m_spawnDelay;
    std::string m_targetName;

	Vector2D<float> getNextSpawnPos(const Vector2D<float>& spawnerPos);
};

