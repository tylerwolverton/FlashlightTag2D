#pragma once
#include "GraphicsComponent.h"

class HealthBarGraphicsComponent :
    public GraphicsComponent
{
public:
    HealthBarGraphicsComponent(ComponentId componentId,
                               std::string texturePath,
                               int animationTimer,
                               std::shared_ptr<TransformComponent> transformComponent);
    virtual ~HealthBarGraphicsComponent();

};

