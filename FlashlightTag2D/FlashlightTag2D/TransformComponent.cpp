#include "TransformComponent.h"

TransformComponent::TransformComponent(Vector2D<float> position, float radius, Vector2D<float> direction)
	: m_position(position),
	  m_size(Vector2D<float>(radius, radius)),
      m_direction(direction)
{
}

TransformComponent::TransformComponent(Vector2D<float> position, Vector2D<float> size, Vector2D<float> direction)
	: m_position(position),
	  m_size(size),
      m_direction(direction)
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