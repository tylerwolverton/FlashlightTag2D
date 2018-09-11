#include "MouseLogicComponent.h"
#include "TransformComponent.h"

MouseLogicComponent::MouseLogicComponent(ComponentId componentId, std::shared_ptr<TransformComponent> transformComponent)
	: LogicComponent(componentId),
	  m_transformComp(transformComponent)
{
}

MouseLogicComponent::~MouseLogicComponent()
{
}

void MouseLogicComponent::UpdateMousePosition(Vector2D<int> mousePos)
{
	m_transformComp->SetPosition(Vector2D<float>(mousePos.x, mousePos.y));
}