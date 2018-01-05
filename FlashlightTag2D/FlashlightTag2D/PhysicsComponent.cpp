#include "PhysicsComponent.h"
#include "TransformComponent.h"

PhysicsComponent::PhysicsComponent(std::shared_ptr<TransformComponent> transformComponent, 
								   float maxSpeed, 
								   float mass, 
                                   Vector2D<float> velocity,
                                   Vector2D<float> acceleration)
	: m_pTransformComponent(transformComponent),
	  m_maxSpeed(maxSpeed),
	  m_mass(mass),
	  m_velocity(velocity),
	  m_acceleration(acceleration),
      m_sumOfForces(Vector2D<float>(0, 0)),
      m_sumOfImpulses(Vector2D<float>(0, 0))
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(GameActor& actor, float deltaMs)
{
    //m_sumOfForces = Vector2D<float>(0, 0);
    //m_sumOfImpulses = Vector2D<float>(0, 0);
	ApplyFriction(0.5f);
	MoveActor(deltaMs);
}

void PhysicsComponent::ApplyFriction(float fricCoeff)
{
    if (m_velocity.Length() > fricCoeff)
    {
        auto dirVec = m_velocity.Normalize();

        AddForce(dirVec * -fricCoeff);
    }
    else
    {
        m_velocity = Vector2D<float>(0,0);
    }
}

void PhysicsComponent::MoveActor(float deltaMs)
{
    auto newPosition = m_pTransformComponent->GetPosition();
    // a = f / m
    m_acceleration = m_sumOfForces / m_mass;
    // v = v0 + a + (impulses / m)
    addVelocity( m_sumOfImpulses / m_mass + m_acceleration * deltaMs);

    m_pTransformComponent->SetPosition(newPosition + m_velocity);

    // Clear out forces and impulses after move
    m_sumOfForces = Vector2D<float>(0, 0);
    m_sumOfImpulses = Vector2D<float>(0, 0);
    m_acceleration = Vector2D<float>(0, 0);
    //m_velocity = Vector2D<float>(0, 0);
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

void PhysicsComponent::SetVelocityToMax()
{
	auto dirVec = m_velocity.Normalize();

	m_velocity = dirVec * m_maxSpeed;
}

void PhysicsComponent::AddImpulse(Vector2D<float> impulse)
{
	// impulse = f * t;
    m_sumOfImpulses += impulse;
}

void PhysicsComponent::AddForce(Vector2D<float> force)
{
    // f = m * a
    m_sumOfForces += force;
}

void PhysicsComponent::addVelocity(Vector2D<float> velocity)
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

ComponentId PhysicsComponent::GetComponentId() const
{
    return ComponentId();
}

EComponentNames PhysicsComponent::GetComponentName() const
{
    return EComponentNames::PhysicsComponentEnum;
}