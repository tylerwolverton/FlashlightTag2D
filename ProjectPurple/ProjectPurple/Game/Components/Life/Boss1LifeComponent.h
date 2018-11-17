#pragma once
#include "LifeComponent.h"

class Boss1LifeComponent :
    public LifeComponent
{
public:
    Boss1LifeComponent(ComponentId componentId, ActorId parentId, int health);
    virtual ~Boss1LifeComponent();

    virtual void TakeDamage(int damage) override;

private:
    uint32_t lastTickVal;

    void updateHealthBar() const;
};

