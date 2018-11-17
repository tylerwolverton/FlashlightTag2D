#include "TransformComponent.h"

TransformComponent::TransformComponent(ComponentId componentId, 
                                       const Vector2D<float>& position, 
                                       float radius, 
                                       const Vector2D<float>& direction)
    : ActorComponent(componentId),
      m_position(position),
      m_initialPosition(position),
      m_size(Vector2D<float>(radius, radius)),
      m_initialSize(Vector2D<float>(radius, radius)),
      m_direction(direction)
{
}

TransformComponent::TransformComponent(ComponentId componentId, 
                                       const Vector2D<float>& position, 
                                       const Vector2D<float>& size, 
                                       const Vector2D<float>& direction)
    : ActorComponent(componentId),
      m_position(position),
      m_initialPosition(position),
      m_size(size),
      m_initialSize(size),
      m_direction(direction)
{
}

TransformComponent::~TransformComponent()
{
}

const EComponentNames TransformComponent::GetComponentName() const
{
    return EComponentNames::TransformComponentEnum;
}