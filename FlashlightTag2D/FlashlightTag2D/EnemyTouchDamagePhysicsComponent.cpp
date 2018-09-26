#include <SDL.h>

#include "EnemyTouchDamagePhysicsComponent.h"
#include "GameActor.h"
#include "LifeComponent.h"
#include "GameStateComponent.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"

EnemyTouchDamagePhysicsComponent::EnemyTouchDamagePhysicsComponent(ComponentId componentId,
                                                                   std::shared_ptr<TransformComponent> transformComponent, 
                                                                   float maxSpeed,
                                                                   float mass,
                                                                   float restitution,
                                                                   Vector2D<float> velocity, 
                                                                   Vector2D<float> acceleration)
	: CharacterPhysicsComponent(componentId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration)
{
}

EnemyTouchDamagePhysicsComponent::~EnemyTouchDamagePhysicsComponent()
{
}

bool EnemyTouchDamagePhysicsComponent::SignalCollision(ActorId actorId)
{
    bool stopResolvingCollisions = false;

    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - lastTickVal > 200)
    {
        std::shared_ptr<GameActor> actor = ServiceLocator::GetActorFactory()->GetActor(actorId);
        if (actor == nullptr)
        {
            return stopResolvingCollisions;
        }

        auto gameStateComponent = actor->GetGameStateComponent();
        if (gameStateComponent != nullptr)
        {
            if (gameStateComponent->GetName() == "Player")
            {
                auto lifeComponent = actor->GetLifeComponent();
                if (lifeComponent != nullptr)
                {
                    lifeComponent->TakeDamage(1);
                }
            }
        }
    }

    return stopResolvingCollisions;
}