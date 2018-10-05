#pragma once
#include "ActorComponent.h"
class LifeComponent :
    public ActorComponent
{
public:
    LifeComponent(ComponentId componentId, ActorId parentId, int health);
    virtual ~LifeComponent();

	virtual const EComponentNames GetComponentName() const override;

    virtual void Die();
    virtual void TakeDamage(int damage);

    void SetHealth(int health) { m_health = health; }

protected:
    ActorId m_parentId;
    int m_health;
};

