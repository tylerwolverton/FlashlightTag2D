#include "GUIItemLogicComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "GameActor.h"
#include "TransformComponent.h"

GUIItemLogicComponent::GUIItemLogicComponent(ComponentId componentId)
    : LogicComponent(componentId),
	  m_cameraTransformComp(nullptr)
{
}

GUIItemLogicComponent::~GUIItemLogicComponent()
{
}

void GUIItemLogicComponent::Update(GameActor& actor, float deltaMs) 
{
    if (m_cameraTransformComp == nullptr)
    {
        m_cameraTransformComp = ServiceLocator::GetActorFactory()->GetCurrentCamera()->GetTransformComponent();
        return;
    }

    auto actorTransformComp = actor.GetTransformComponent();
    actorTransformComp->SetPosition(actorTransformComp->GetInitPosition() 
                                    + m_cameraTransformComp->GetPosition());
}