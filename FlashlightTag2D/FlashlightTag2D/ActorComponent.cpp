#include "ActorComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

const std::shared_ptr<GameActor> ActorComponent::GetParent() const
{
    auto actorFactory = ServiceLocator::GetActorFactory();
    if (actorFactory != nullptr)
    {
        return actorFactory->GetActor(m_parentActorId);
    }

    return nullptr;
}
