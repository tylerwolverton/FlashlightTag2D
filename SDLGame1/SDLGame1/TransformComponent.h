#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class TransformComponent :
	public ActorComponent
{
public:
	TransformComponent(Vector2D<float> position, float radius);
	TransformComponent(Vector2D<float> position, Vector2D<float> size);
	virtual ~TransformComponent();

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

	Vector2D<float> GetPosition() { return m_position; }
	void SetPosition(Vector2D<float> position) { m_position = position; }

	Vector2D<float> GetSize() { return m_size; }
	void SetSize(Vector2D<float> size) { m_size = size; }

	float GetRadius() { return m_size.x; }
	void SetRadius(float radius) { m_size.x = radius; m_size.y = radius; }

private:
	Vector2D<float> m_position;
	Vector2D<float> m_size;
};

