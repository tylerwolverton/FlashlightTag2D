#include "GUIItemLogicComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "GameActor.h"
#include "TransformComponent.h"

GUIItemLogicComponent::GUIItemLogicComponent(ComponentId componentId)
    : LogicComponent(componentId),
      m_cameraTransformCompPtr(nullptr)
{
}

GUIItemLogicComponent::~GUIItemLogicComponent()
{
}

void GUIItemLogicComponent::Update(GameActor& actor, float deltaMs) 
{
    if (m_cameraTransformCompPtr == nullptr)
    {
		auto actorFactoryPtr = ServiceLocator::GetActorFactory();
		if (actorFactoryPtr == nullptr)
		{
			return;
		}

        m_cameraTransformCompPtr = actorFactoryPtr->GetCurrentCamera()->GetTransformCompPtr();
    }

    auto actorTransformCompPtr = actor.GetTransformCompPtr();
    actorTransformCompPtr->SetPosition(actorTransformCompPtr->GetInitPosition()
										+ m_cameraTransformCompPtr->GetPosition());
}