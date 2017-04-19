#include "PhysicsComponent.h"
#include "TransformComponent.h"

PhysicsComponent::PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent)
	: m_TransformComponent(transformComponent),
	  m_velocity(Vector2D<float>(0, 0)),
	  m_maxSpeed(10),
	  m_mass(1)
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(GameActor& actor, int deltaMs)
{
	MoveActor();
	//ApplyFriction(0.1f);
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
	AddVelocity(force / m_mass);
}

void PhysicsComponent::ApplyFriction(float fricCoeff)
{
	auto dirVec = m_velocity.Normalize();

	AddForce(-dirVec);// *-fricCoeff);
}

void PhysicsComponent::MoveActor()
{
	m_TransformComponent->SetPosition(m_TransformComponent->GetPosition() + m_velocity);
}