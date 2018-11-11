#include "MouseLogicComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "GraphicsManager.h"
#include "GameActor.h"
#include "World.h"

MouseLogicComponent::MouseLogicComponent(ComponentId componentId, std::shared_ptr<TransformComponent> transformCompPtr)
    : LogicComponent(componentId),
      m_transformCompPtr(transformCompPtr),
      m_cameraTransformCompPtr(nullptr)
{
}

MouseLogicComponent::~MouseLogicComponent()
{
}

void MouseLogicComponent::UpdateMousePosition(Vector2D<int> mousePos)
{
    if (m_cameraTransformCompPtr == nullptr)
    {
        m_cameraTransformCompPtr = ServiceLocator::GetGraphicsManager()->GetCurrentCamera()->GetTransformCompPtr();
    }

    m_transformCompPtr->SetPosition(Vector2D<float>(mousePos.x, World::SCREEN_HEIGHT - mousePos.y) + m_cameraTransformCompPtr->GetPosition());
}