#include "ActorComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

const std::shared_ptr<GameActor> ActorComponent::GetParent() const
{
    auto actorFactoryPtr = ServiceLocator::GetActorFactory();
    if (actorFactoryPtr != nullptr)
    {
        return actorFactoryPtr->GetActor(m_parentActorId);
    }

    return nullptr;
}
