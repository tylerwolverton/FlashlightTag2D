#pragma once
#include "LogicComponent.h"

class TransformComponent;

class MouseLogicComponent :
    public LogicComponent
{
public:
    MouseLogicComponent(ComponentId componentId, const std::shared_ptr<TransformComponent>& transformCompPtr);
    virtual ~MouseLogicComponent();

    void UpdateMousePosition(const Vector2D<int>& mousePos) override;

private:
    std::shared_ptr<TransformComponent> m_transformCompPtr;
    std::shared_ptr<TransformComponent> m_cameraTransformCompPtr;
};

