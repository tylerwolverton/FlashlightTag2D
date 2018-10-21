#include "HealthBarGraphicsComponent.h"

HealthBarGraphicsComponent::HealthBarGraphicsComponent(ComponentId componentId,
                                                       std::string texturePath,
                                                       int animationTimer,
                                                       std::shared_ptr<TransformComponent> transformComponent)
    : GraphicsComponent(componentId, texturePath, animationTimer, transformComponent)
{
}

HealthBarGraphicsComponent::~HealthBarGraphicsComponent()
{
}
