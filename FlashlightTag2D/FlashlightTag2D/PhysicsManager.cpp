#include "PhysicsManager.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include <stdlib.h>
#include <algorithm>

PhysicsManager::PhysicsManager()
    : m_lastComponentId(0)
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Update(float deltaTime)
{
	ResolveCollisions(deltaTime);
}

void PhysicsManager::ResolveCollisions(float deltaTime)
{
	for (auto actorPhysicsComponent : m_physicsComponentPtrVec)
	{
        std::shared_ptr<TransformComponent> actorTransformComponent = actorPhysicsComponent->GetTransformComponent();

		for (auto innerActorPhysicsComponent : m_physicsComponentPtrVec)
		{
			if (actorPhysicsComponent->GetComponentId() != innerActorPhysicsComponent->GetComponentId())
			{
                std::shared_ptr<TransformComponent> innerTransformComponent = innerActorPhysicsComponent->GetTransformComponent();

                auto collisionEvent = checkCircleCollision(actorTransformComponent, innerTransformComponent);
				if (collisionEvent.penetrationDepth < 0)
				{
                    resolvePenetration(actorTransformComponent, innerTransformComponent, collisionEvent);
                    resolveCollision(actorPhysicsComponent, innerActorPhysicsComponent, collisionEvent);
                    //actorPhysicsComponent.SignalCollision(*innerActor);
                    //innerActorPhysicsComponent.SignalCollision(*actor);
				}
			}
		}
	}
}

void PhysicsManager::AddPhysicsComponentPtr(StrongPhysicsComponentPtr comp)
{
    m_physicsComponentPtrVec.push_back(comp);
}

// TODO: Cache changes
//int PhysicsManager::AddPlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent,
//                                              float maxSpeed,
//                                              float mass,
//                                              float restitution,
//                                              Vector2D<float> velocity,
//                                              Vector2D<float> acceleration)
//{
//    int compId = getNextComponentId();
//    m_physicsComponentVec.emplace_back(compId, transformComponent, maxSpeed, mass, restitution, velocity, acceleration);
//    return compId;
//}

PhysicsManager::CollisionEvent PhysicsManager::checkCircleCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent)
{
	Vector2D<float> dist = actorTransformComponent->GetPosition() - innerActorTransformComponent->GetPosition();
	float sizeSum = actorTransformComponent->GetRadius() + innerActorTransformComponent->GetRadius();

    auto collisionEvent = CollisionEvent{ dist.Length() - sizeSum, dist.Normalize() };

	return collisionEvent;
}

void PhysicsManager::resolvePenetration(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent, const PhysicsManager::CollisionEvent& collisionEvent)
{
    auto moveDist = abs(collisionEvent.penetrationDepth) / 2;

    actorTransformComponent->SetPosition(actorTransformComponent->GetPosition() + collisionEvent.normal * moveDist);
    innerActorTransformComponent->SetPosition(innerActorTransformComponent->GetPosition() - collisionEvent.normal * moveDist);
}

void PhysicsManager::resolveCollision(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent)
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