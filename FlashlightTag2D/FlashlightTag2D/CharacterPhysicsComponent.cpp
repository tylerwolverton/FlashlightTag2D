#include "CharacterPhysicsComponent.h"
#include "GameActor.h"
#include "TransformComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

CharacterPhysicsComponent::CharacterPhysicsComponent(ComponentId componentId,
                                                     std::shared_ptr<TransformComponent> transformComponent, 
                                                     float maxSpeed,
                                                     float mass,
                                                     float restitution,
                                                     Vector2D<float> velocity, 
                                                     Vector2D<float> acceleration)
	: PhysicsComponent(componentId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
{
}

CharacterPhysicsComponent::~CharacterPhysicsComponent()
{
}

bool CharacterPhysicsComponent::SignalCollision(ActorId actorId)
{
    bool stopResolvingCollisions = false;
    std::shared_ptr<GameActor> actor = ServiceLocator::GetActorFactory()->GetActor(actorId);
    if (actor == nullptr)
    {
        return stopResolvingCollisions;
    }
    
    return stopResolvingCollisions;
}