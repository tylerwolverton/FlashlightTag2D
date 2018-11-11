#pragma once
#include "Behavior.h"

class GameActor;

class ShootAtTargetBehavior :
    public Behavior
{
public:
    ShootAtTargetBehavior(std::shared_ptr<GameActor> targetActorPtr = nullptr, int cooldown = 4);
    virtual ~ShootAtTargetBehavior();

    std::vector<std::shared_ptr<Command>> Update(const GameActor& actor) override;

    void SetTarget(std::shared_ptr<GameActor> targetActor) { m_targetActorPtr = targetActor; }

private:
    std::shared_ptr<GameActor> m_targetActorPtr;
    uint32_t m_lastTickVal;
    int m_cooldown;
};

