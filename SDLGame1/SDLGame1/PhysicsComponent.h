#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class TransformComponent;

class PhysicsComponent :
	public ActorComponent
{
public:
	PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent, Vector2D<float> velocity, float maxSpeed, float mass);
	virtual ~PhysicsComponent();

	void Update(GameActor& actor, int deltaMs) override;
	virtual void SignalCollision(GameActor& actor) {};

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

	std::shared_ptr<TransformComponent> GetTransformComponent() { return m_TransformComponent; }

	Vector2D<float> GetVelocity() { return m_velocity; }
	void SetVelocity(Vector2D<float> newVelocity);
	void AddVelocity(Vector2D<float> velocity);
	void SetVelocityToMax();

	void AddForce(Vector2D<float> force);

	void ApplyFriction(float fricCoeff);
	void MoveActor();

protected:
	std::shared_ptr<TransformComponent> m_TransformComponent;
	Vector2D<float> m_velocity;
	float m_maxSpeed;
	float m_mass;
};

