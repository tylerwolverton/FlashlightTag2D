#include "PhysicsManager.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include <stdlib.h>

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
					//moveActors(actorPhysicsComponent, innerActorPhysicsComponent, -collisionEvent.penetrationDepth, deltaTime);
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

void PhysicsManager::resolvePenetration(StrongGameActorPtr actor, StrongGameActorPtr innerActor, PhysicsManager::CollisionEvent collisionEvent)
{
    auto actorTransformComponent = actor->GetTransformComponent();
    auto innerActorTransformComponent = innerActor->GetTransformComponent();

    auto moveDist = abs(collisionEvent.penetrationDepth) / 2;// / 2 + .02f;

    actorTransformComponent->SetPosition(actorTransformComponent->GetPosition() + collisionEvent.normal * moveDist);
    innerActorTransformComponent->SetPosition(innerActorTransformComponent->GetPosition() - collisionEvent.normal * moveDist);
}

void PhysicsManager::moveActors(std::shared_ptr<PhysicsComponent>& actorPhysicsComp, std::shared_ptr<PhysicsComponent>& innerActorPhysicsComp, float penetrationDepth, float deltaTime)
{
	auto actorMomentum = actorPhysicsComp->GetMomentum();
	auto innerActorMomentum = innerActorPhysicsComp->GetMomentum();

	// vrel = v1 - v2
	// v1' = v1 - kn/m1
	// v2' = v2 + kn/m2
	// k = (e+1)vrel.Dot(n) / (1/m1 + 1/m2)n.Dot(n)

	/*actorPhysicsComp->AddForce(innerActorPhysicsComp->GetVelocity() * innerActorPhysicsComp->GetMass() / deltaTime / 2);
	innerActorPhysicsComp->AddForce(actorPhysicsComp->GetVelocity() * actorPhysicsComp->GetMass() / deltaTime / 2);*/

	/*actorPhysicsComp->SetVelocity((innerActorMomentum / actorPhysicsComp->GetMass()) / deltaTime);
	innerActorPhysicsComp->SetVelocity((actorMomentum / innerActorPhysicsComp->GetMass()) / deltaTime);*/
    //auto actorMomentum = actorPhysicsComp->GetVelocity() * actorPhysicsComp->GetMass();
    //auto innerActorMomentum = innerActorPhysicsComp->GetVelocity() * innerActorPhysicsComp->GetMass();

    actorPhysicsComp->AddImpulse(-actorMomentum);
    actorPhysicsComp->AddImpulse(innerActorMomentum);
    innerActorPhysicsComp->AddImpulse(-innerActorMomentum);
    innerActorPhysicsComp->AddImpulse(actorMomentum);
}