#include "PhysicsManager.h"
#include "TransformComponent.h"
#include "GameActor.h"

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Update(StrongGameActorPtrList gameActors)
{

}

void PhysicsManager::ResolveCollisions(StrongGameActorPtrList gameActors)
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

				if (CheckCircleCollision(actor, innerActor))
				{
					MoveActors(actorPhysicsComponent, innerActorPhysicsComponent);
                    actorPhysicsComponent->SignalCollision(*innerActor);
                    innerActorPhysicsComponent->SignalCollision(*actor);
				}
			}
		}
	}
}

bool PhysicsManager::CheckCircleCollision(StrongGameActorPtr actor, StrongGameActorPtr innerActor)
{
	auto actorTransformComponent = actor->GetTransformComponent();
    auto innerActorTransformComponent = innerActor->GetTransformComponent();

	Vector2D<float> dist = actorTransformComponent->GetPosition() - innerActorTransformComponent->GetPosition();
	float sizeSum = actorTransformComponent->GetRadius() + innerActorTransformComponent->GetRadius();

	return dist.Length() < sizeSum;
}

void PhysicsManager::MoveActors(std::shared_ptr<PhysicsComponent>& actorPhysicsComp, std::shared_ptr<PhysicsComponent>& innerActorPhysicsComp)
{
	auto actorMomentum = actorPhysicsComp->GetMomentum();
	auto innerActorMomentum = innerActorPhysicsComp->GetMomentum();

	actorPhysicsComp->SetVelocity(innerActorMomentum / actorPhysicsComp->GetMass());
	innerActorPhysicsComp->SetVelocity(actorMomentum / innerActorPhysicsComp->GetMass());
}