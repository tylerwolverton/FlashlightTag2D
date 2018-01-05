#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class TransformComponent;

class PhysicsComponent :
	public ActorComponent
{
public:
	PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent, 
					float maxSpeed, 
					float mass, 
					Vector2D<float> velocity = Vector2D<float>(0, 0), 
					Vector2D<float> acceleration = Vector2D<float>(0, 0));
	virtual ~PhysicsComponent();

	void Update(GameActor& actor, float deltaMs) override;
	virtual void SignalCollision(GameActor& actor) {};

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

	std::shared_ptr<TransformComponent> GetTransformComponent() { return m_pTransformComponent; }

	Vector2D<float> GetVelocity() { return m_velocity; }
	void SetVelocity(Vector2D<float> newVelocity);
	void SetVelocityToMax();

	float GetMass() { return m_mass; }
	Vector2D<float> GetMomentum() { return m_velocity * m_mass; }

	void AddImpulse(Vector2D<float> impulse);
	void AddForce(Vector2D<float> force);

	void ApplyFriction(float fricCoeff);
	void MoveActor(float deltaMs);

protected:
	std::shared_ptr<TransformComponent> m_pTransformComponent;
	Vector2D<float> m_velocity;
	Vector2D<float> m_acceleration;
    Vector2D<float> m_sumOfForces;
    Vector2D<float> m_sumOfImpulses;
	float m_maxSpeed;
	float m_mass;

private:
	void addVelocity(Vector2D<float> velocity);
};

