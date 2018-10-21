#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class TransformComponent :
	public ActorComponent
{
public:
	TransformComponent(ComponentId componentId, Vector2D<float> position, float radius, Vector2D<float> direction);
	TransformComponent(ComponentId componentId, Vector2D<float> position, Vector2D<float> size, Vector2D<float> direction);
	virtual ~TransformComponent();

	//virtual ComponentId GetComponentId() const override;
	virtual const EComponentNames GetComponentName() const override;

	Vector2D<float> GetPosition() { return m_position; }
    Vector2D<float> GetInitPosition() { return m_initialPosition; }
	void SetPosition(Vector2D<float> position) { m_position = position; }

    Vector2D<float> GetInitSize() { return m_initialSize; }
	Vector2D<float> GetSize() { return m_size; }
	void SetSize(Vector2D<float> size) { m_size = size; }

    Vector2D<float> GetDirection() { return m_direction; }
    void SetDirection(Vector2D<float> direction) { m_direction = direction; }

	float GetRadius() { return m_size.x / 2; }
	void SetRadius(float radius) { m_size.x = radius * 2; m_size.y = radius * 2; }

private:
    Vector2D<float> m_initialPosition;
	Vector2D<float> m_position;
    Vector2D<float> m_initialSize;
    Vector2D<float> m_size;
    Vector2D<float> m_direction;
};

