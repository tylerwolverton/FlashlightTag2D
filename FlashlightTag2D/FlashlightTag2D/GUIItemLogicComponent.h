#pragma once
#include "LogicComponent.h"

class TransformComponent;

class GUIItemLogicComponent :
    public LogicComponent
{
public:
    GUIItemLogicComponent(ComponentId componentId);
    virtual ~GUIItemLogicComponent();

    virtual void Update(GameActor& actor, float deltaMs) override;

private:
    std::shared_ptr<TransformComponent> m_cameraTransformComp;
};

