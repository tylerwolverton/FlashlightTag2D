#include "MouseLogicComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "GraphicsManager.h"
#include "GameActor.h"
#include "World.h"

MouseLogicComponent::MouseLogicComponent(ComponentId componentId, std::shared_ptr<TransformComponent> transformComponent)
    : LogicComponent(componentId),
      m_transformComp(transformComponent),
      m_cameraTransformComp(nullptr)
{
}

MouseLogicComponent::~MouseLogicComponent()
{
}

void MouseLogicComponent::UpdateMousePosition(Vector2D<int> mousePos)
{
    if (m_cameraTransformComp == nullptr)
    {
        m_cameraTransformComp = ServiceLocator::GetGraphicsManager()->GetCurrentCamera()->GetTransformComponent();
    }

    m_transformComp->SetPosition(Vector2D<float>(mousePos.x, World::SCREEN_HEIGHT - mousePos.y) + m_cameraTransformComp->GetPosition());
}