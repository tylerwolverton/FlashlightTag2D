#pragma once
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "Behavior.h"

class BaseLogicComponent :
	public ActorComponent
{
public:
	BaseLogicComponent(std::shared_ptr<TransformComponent> transformComponent);
	virtual ~BaseLogicComponent();

	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft(); 

	virtual void Update(GameActor& actor, int deltaMs) {}

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

private:
	std::shared_ptr<TransformComponent> m_TransformComponent;
	std::shared_ptr<Behavior> behavior;
};

