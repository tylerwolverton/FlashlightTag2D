#pragma once
#include "ActorComponent.h"

class PhysicsComponent;

class LogicComponent :
	public ActorComponent
{
public:
	LogicComponent(ComponentId componentId);// , std::shared_ptr<PhysicsComponent> physicsComponent);
    LogicComponent(int actorId, ComponentId componentId);
    virtual ~LogicComponent();

	virtual void MoveUp() {}
	virtual void MoveDown() {}
	virtual void MoveRight() {}
	virtual void MoveLeft() {}
	virtual void Select() {}
	virtual void Shoot() {}
	virtual void UpdateMousePosition(Vector2D<int> mousePos) {}

	virtual void Update(GameActor& actor, float deltaMs) {}

	virtual const EComponentNames GetComponentName() const override;

protected:
	//std::shared_ptr<PhysicsComponent> m_pPhysicsComponent;

private:
    int m_actorId;
};

