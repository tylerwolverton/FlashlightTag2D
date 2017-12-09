#pragma once
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
//#include "Behavior.h"

class BaseLogicComponent :
	public ActorComponent
{
public:
	BaseLogicComponent(std::shared_ptr<TransformComponent> transformComponent);
	BaseLogicComponent(std::shared_ptr<PhysicsComponent> physicsComponent);
	virtual ~BaseLogicComponent();

	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft(); 

	virtual void Update(GameActor& actor, int deltaMs) {}

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

private:
	std::shared_ptr<TransformComponent> m_pTransformComponent;
	std::shared_ptr<PhysicsComponent> m_pPhysicsComponent;
	//std::shared_ptr<Behavior> m_pBehavior;
};

