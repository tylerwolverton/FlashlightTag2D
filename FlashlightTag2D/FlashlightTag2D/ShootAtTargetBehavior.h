#pragma once
#include "Behavior.h"

class GameActor;

class ShootAtTargetBehavior :
    public Behavior
{
public:
    ShootAtTargetBehavior(std::shared_ptr<GameActor> targetActor = nullptr, int cooldown = 4);
    virtual ~ShootAtTargetBehavior();

    std::vector<std::shared_ptr<Command>> Update(const GameActor& actor) override;

    void SetTarget(std::shared_ptr<GameActor> targetActor) { m_targetActor = targetActor; }

private:
    std::shared_ptr<GameActor> m_targetActor;
    uint32_t m_lastTickVal;
    int m_cooldown;
};

