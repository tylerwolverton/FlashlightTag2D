#include "PhysicsComponent.h"
#include "TransformComponent.h"

PhysicsComponent::PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent, Vector2D<float> velocity, float maxSpeed, float mass)
	: m_pTransformComponent(transformComponent),
	  m_velocity(velocity),
	  m_maxSpeed(maxSpeed),
	  m_mass(mass)
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(GameActor& actor, int deltaMs)
{
	ApplyFriction(0.5f);
	MoveActor();
}

ComponentId PhysicsComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames PhysicsComponent::GetComponentName() const
{
	return EComponentNames::PhysicsComponentEnum;
}

void PhysicsComponent::SetVelocity(Vector2D<float> newVelocity)
{
	m_velocity = newVelocity;
	if (m_velocity.Length() > m_maxSpeed)
	{
		SetVelocityToMax();
	}
	if (m_velocity.Length() < 0.1f)
	{
		m_velocity = Vector2D<float>(0, 0);
	}
}

void PhysicsComponent::AddVelocity(Vector2D<float> velocity)
{
	m_velocity += velocity;
	if (m_velocity.Length() > m_maxSpeed)
	{
		SetVelocityToMax();
	}
	if (m_velocity.Length() < 0.1f)
	{
		m_velocity = Vector2D<float>(0, 0);
	}
}

void PhysicsComponent::SetVelocityToMax()
{
	auto dirVec = m_velocity.Normalize();

	m_velocity = dirVec * m_maxSpeed;
}

void PhysicsComponent::AddForce(Vector2D<float> force)
{
	//f = ma;
	if(force.Length() > 0)
		AddVelocity(force / m_mass);
}

void PhysicsComponent::ApplyFriction(float fricCoeff)
{
	auto dirVec = m_velocity.Normalize();

	AddForce(dirVec * -fricCoeff);
}

void PhysicsComponent::MoveActor()
{
	m_pTransformComponent->SetPosition(m_pTransformComponent->GetPosition() + m_velocity);
}