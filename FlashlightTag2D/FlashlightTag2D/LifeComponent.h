#pragma once
#include "ActorComponent.h"
class LifeComponent :
    public ActorComponent
{
public:
    LifeComponent(ComponentId componentId, ActorId parentId, int health);
    virtual ~LifeComponent();

    void Die();
    void TakeDamage(int damage);

private:
    ActorId m_parentId;
    int m_health;
};

