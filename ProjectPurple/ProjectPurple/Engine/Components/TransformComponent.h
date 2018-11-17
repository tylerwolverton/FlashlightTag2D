#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class TransformComponent :
    public ActorComponent
{
public:
    TransformComponent(ComponentId componentId, 
                       const Vector2D<float>& position, 
                       float radius, 
                       const Vector2D<float>& direction);
    TransformComponent(ComponentId componentId, const Vector2D<float>& position, const Vector2D<float>& size, const Vector2D<float>& direction);
    virtual ~TransformComponent();

    virtual const EComponentNames GetComponentName() const override;

    Vector2D<float> GetPosition() const { return m_position; }
    Vector2D<float> GetInitPosition() const { return m_initialPosition; }
    void SetPosition(const Vector2D<float>& position) { m_position = position; }

    Vector2D<float> GetInitSize() const { return m_initialSize; }
    Vector2D<float> GetSize() const { return m_size; }
    void SetSize(const Vector2D<float>& size) { m_size = size; }

    Vector2D<float> GetDirection() const { return m_direction; }
    void SetDirection(const Vector2D<float>& direction) { m_direction = direction; }

    float GetRadius() const { return m_size.x / 2; }
    void SetRadius(float radius) { m_size.x = radius * 2; m_size.y = radius * 2; }

private:
    Vector2D<float> m_initialPosition;
    Vector2D<float> m_position;
    Vector2D<float> m_initialSize;
    Vector2D<float> m_size;
    Vector2D<float> m_direction;
};

