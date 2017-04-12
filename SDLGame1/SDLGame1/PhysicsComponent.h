#pragma once
#include "ActorComponent.h"

class TransformComponent;

class PhysicsComponent :
	public ActorComponent
{
public:
	PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent);
	virtual ~PhysicsComponent();

	virtual void SignalCollision(GameActor& actor) {};

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

protected:
	std::shared_ptr<TransformComponent> m_TransformComponent;

};

