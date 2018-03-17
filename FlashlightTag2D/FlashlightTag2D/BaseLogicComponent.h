#pragma once
#include "ActorComponent.h"

class TransformComponent;
class PhysicsComponent;

class BaseLogicComponent :
	public ActorComponent
{
public:
	BaseLogicComponent(ComponentId componentId, std::shared_ptr<TransformComponent> transformComponent);
	BaseLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent);
	virtual ~BaseLogicComponent();

	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft(); 

	virtual void Update(GameActor& actor, float deltaMs) {}

	virtual const EComponentNames GetComponentName() const override;

private:
	std::shared_ptr<TransformComponent> m_pTransformComponent;
	std::shared_ptr<PhysicsComponent> m_pPhysicsComponent;
};

