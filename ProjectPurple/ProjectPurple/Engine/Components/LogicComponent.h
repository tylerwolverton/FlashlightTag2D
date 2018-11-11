#pragma once
#include "ActorComponent.h"

class PhysicsComponent;

class LogicComponent :
    public ActorComponent
{
public:
    LogicComponent(ComponentId componentId);
    LogicComponent(int actorId, ComponentId componentId);
    virtual ~LogicComponent();

    virtual void MoveUp() {}
    virtual void MoveDown() {}
    virtual void MoveRight() {}
    virtual void MoveLeft() {}
    virtual void Select() {}
    virtual void Shoot() {}
    virtual void Spawn() {}
    virtual void UpdateMousePosition(Vector2D<int> mousePos) {}

    virtual void Update(GameActor& actor, float deltaMs) {}

    virtual const EComponentNames GetComponentName() const override;

private:
    int m_actorId;
};

