#include "BaseLogicComponent.h"

BaseLogicComponent::BaseLogicComponent(std::shared_ptr<TransformComponent> transformComponent)
	: m_TransformComponent(transformComponent)
{
}

BaseLogicComponent::~BaseLogicComponent()
{
}

bool BaseLogicComponent::Init(tinyxml2::XMLElement* pData)
{
	return true;
}

ComponentId BaseLogicComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames BaseLogicComponent::GetComponentName() const
{
	return EComponentNames::BaseLogicComponentEnum;
}