#include "TransformComponent.h"

TransformComponent::TransformComponent(Vector2D<float> position, float radius)
	: m_position(position),
	  m_size(Vector2D<float>(radius, radius))
{
}

TransformComponent::TransformComponent(Vector2D<float> position, Vector2D<float> size)
	: m_position(position),
	  m_size(size)
{
}

TransformComponent::~TransformComponent()
{
}

ComponentId TransformComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames TransformComponent::GetComponentName() const
{
	return EComponentNames::TransformComponentEnum;
}