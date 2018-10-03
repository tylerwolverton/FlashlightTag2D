#pragma once
#include "LifeComponent.h"

class PlayerLifeComponent :
    public LifeComponent
{
public:
    PlayerLifeComponent(ComponentId componentId, ActorId parentId, int health);
    virtual ~PlayerLifeComponent();

    virtual void TakeDamage(int damage) override;

private:
    uint32_t lastTickVal;
};
