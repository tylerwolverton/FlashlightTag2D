#include "PhysicsManager.h"
#include "PhysicsComponent.h"
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
	for (auto& actor : gameActors)
	{
		StrongActorComponentPtr actorPhysicsComponent = actor->GetComponentByName(EComponentNames::PhysicsComponentEnum);
		if (actorPhysicsComponent == nullptr)
		{
			continue;
		}

		std::shared_ptr<PhysicsComponent> rawActorPhysicsComponent = std::dynamic_pointer_cast<PhysicsComponent>(actorPhysicsComponent);
		
		for (auto& innerActor : gameActors)
		{
			if (actor != innerActor)
			{
				StrongActorComponentPtr innerActorPhysicsComponent = innerActor->GetComponentByName(EComponentNames::PhysicsComponentEnum);
				if (innerActorPhysicsComponent == nullptr)
				{
					continue;
				}

				std::shared_ptr<PhysicsComponent> rawInnerActorPhysicsComponent = std::dynamic_pointer_cast<PhysicsComponent>(innerActorPhysicsComponent);

				if (CheckCircleCollision(actor, innerActor))
				{
					rawActorPhysicsComponent->SignalCollision(*innerActor);
					rawInnerActorPhysicsComponent->SignalCollision(*actor);
				}
			}
		}
	}
}

bool PhysicsManager::CheckCircleCollision(StrongGameActorPtr actor, StrongGameActorPtr innerActor)
{
	std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actor->GetComponentByName(TransformComponentEnum));
	std::shared_ptr<TransformComponent> rawInnerActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(innerActor->GetComponentByName(TransformComponentEnum));
	
	Vector2D<float> dist = rawActorTransformComponent->GetPosition() - rawInnerActorTransformComponent->GetPosition();
	float sizeSum = rawActorTransformComponent->GetRadius() + rawInnerActorTransformComponent->GetRadius();

	return dist.Length() < sizeSum;
}