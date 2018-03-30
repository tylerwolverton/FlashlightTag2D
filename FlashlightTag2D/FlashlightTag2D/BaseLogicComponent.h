#pragma once
#include "ActorComponent.h"

class PhysicsComponent;

class BaseLogicComponent :
	public ActorComponent
{
public:
    BaseLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent);
    BaseLogicComponent(int actorId, ComponentId componentId);
    virtual ~BaseLogicComponent();

	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft(); 

	virtual void Update(GameActor& actor, float deltaMs) {}

	virtual const EComponentNames GetComponentName() const override;

private:
    int m_actorId;
	std::shared_ptr<PhysicsComponent> m_pPhysicsComponent;
};

