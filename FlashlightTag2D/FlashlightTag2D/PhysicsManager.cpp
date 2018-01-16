#include "PhysicsManager.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include <stdlib.h>
#include <algorithm>

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Update(StrongGameActorPtrList gameActors, float deltaTime)
{
	ResolveCollisions(gameActors, deltaTime);
}

void PhysicsManager::ResolveCollisions(StrongGameActorPtrList gameActors, float deltaTime)
{
	for (const auto& actor : gameActors)
	{
		auto actorPhysicsComponent = actor->GetPhysicsComponent();
		if (actorPhysicsComponent == nullptr)
		{
			continue;
		}

		for (const auto& innerActor : gameActors)
		{
			if (actor != innerActor)
			{
				auto innerActorPhysicsComponent = innerActor->GetPhysicsComponent();
				if (innerActorPhysicsComponent == nullptr)
				{
					continue;
				}

                auto collisionEvent = checkCircleCollision(actor, innerActor);
				if (collisionEvent.penetrationDepth < 0)
				{
                    resolvePenetration(actor, innerActor, collisionEvent);
                    resolveCollision(actorPhysicsComponent, innerActorPhysicsComponent, collisionEvent);
                    actorPhysicsComponent->SignalCollision(*innerActor);
                    innerActorPhysicsComponent->SignalCollision(*actor);
				}
			}
		}
	}
}

PhysicsManager::CollisionEvent PhysicsManager::checkCircleCollision(StrongGameActorPtr actor, StrongGameActorPtr innerActor)
{
	auto actorTransformComponent = actor->GetTransformComponent();
    auto innerActorTransformComponent = innerActor->GetTransformComponent();

	Vector2D<float> dist = actorTransformComponent->GetPosition() - innerActorTransformComponent->GetPosition();
	float sizeSum = actorTransformComponent->GetRadius() + innerActorTransformComponent->GetRadius();

    auto collisionEvent = CollisionEvent{ dist.Length() - sizeSum, dist.Normalize() };

	return collisionEvent;
}

void PhysicsManager::resolvePenetration(StrongGameActorPtr actor, StrongGameActorPtr innerActor, const PhysicsManager::CollisionEvent& collisionEvent)
{
    auto actorTransformComponent = actor->GetTransformComponent();
    auto innerActorTransformComponent = innerActor->GetTransformComponent();

    auto moveDist = abs(collisionEvent.penetrationDepth) / 2;

    actorTransformComponent->SetPosition(actorTransformComponent->GetPosition() + collisionEvent.normal * moveDist);
    innerActorTransformComponent->SetPosition(innerActorTransformComponent->GetPosition() - collisionEvent.normal * moveDist);
}

void PhysicsManager::resolveCollision(StrongPhysicsComponentPtr actorPhysicsComp, StrongPhysicsComponentPtr innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent)
{
    auto relativeVelocity = innerActorPhysicsComp->GetVelocity() - actorPhysicsComp->GetVelocity();

    auto velAlongNormal = relativeVelocity.Dot(collisionEvent.normal);

    // Do not resolve if velocities are separating
//    if (velAlongNormal > 0)
 //       return;

    // Calculate restitution
    float e = std::min(actorPhysicsComp->GetRestitution(), innerActorPhysicsComp->GetRestitution());

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= 1 / actorPhysicsComp->GetMass() + 1 / innerActorPhysicsComp->GetMass();

    // Apply impulse
    auto impulse = j * collisionEvent.normal;
    actorPhysicsComp->AddForce(-impulse);
    innerActorPhysicsComp->AddForce(impulse);
}