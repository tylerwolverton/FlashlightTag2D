#include "PhysicsManager.h"
#include "TransformComponent.h"
//#include "GameActor.h"
#include <stdlib.h>
#include <algorithm>

PhysicsManager::PhysicsManager()
    : m_lastComponentId(0),
      m_levelSize(Vector2D<int>(-1, -1))
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::ClearPhysicsComponents()
{
    //m_physicsComponentPtrVec.clear();
    m_physicsComponentPtrMap.clear();
}

void PhysicsManager::Update(float deltaTime)
{
	ResolveCollisions(deltaTime);
    moveActorsBackIntoLevel();
}

void PhysicsManager::ResolveCollisions(float deltaTime)
{
    //for (auto actorPhysicsComponent : m_physicsComponentPtrVec)
    for (auto actorPhysicsComponent : m_physicsComponentPtrMap)
    {
        std::shared_ptr<TransformComponent> actorTransformComponent = actorPhysicsComponent.second->GetTransformComponent();

		for (auto innerActorPhysicsComponent : m_physicsComponentPtrMap)
		{
			if (actorPhysicsComponent.second->GetComponentId() != innerActorPhysicsComponent.second->GetComponentId())
			{
                std::shared_ptr<TransformComponent> innerTransformComponent = innerActorPhysicsComponent.second->GetTransformComponent();

                auto collisionEvent = checkCircleCollision(actorTransformComponent, innerTransformComponent);
				if (collisionEvent.penetrationDepth < 0)
				{
                    resolvePenetration(actorTransformComponent, innerTransformComponent, collisionEvent);
                    resolveCollision(actorPhysicsComponent.second, innerActorPhysicsComponent.second, collisionEvent);
                    actorPhysicsComponent.second->SignalCollision(innerActorPhysicsComponent.second->GetParentActorId());
                    innerActorPhysicsComponent.second->SignalCollision(actorPhysicsComponent.second->GetParentActorId());
				}
			}
		}
	}
}

void PhysicsManager::AddPhysicsComponentPtr(ComponentId compId, std::shared_ptr<PhysicsComponent> comp)
{
    //m_physicsComponentPtrVec.push_back(comp);
    m_physicsComponentPtrMap.insert(std::make_pair(compId, comp));
}

void PhysicsManager::RemovePhysicsComponentPtr(ComponentId compId)
{
    m_physicsComponentPtrMap.erase(compId);
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
    Vector2D<float> relativeVelocity = innerActorPhysicsComp->GetVelocity() - actorPhysicsComp->GetVelocity();

    float velAlongNormal = relativeVelocity.Dot(collisionEvent.normal);

    // Do not resolve if velocities are separating
//    if (velAlongNormal > 0)
 //       return;

    // Calculate restitution
    float e = std::min(actorPhysicsComp->GetRestitution(), innerActorPhysicsComp->GetRestitution());

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= 1 / actorPhysicsComp->GetMass() + 1 / innerActorPhysicsComp->GetMass();

    // Apply impulse
    Vector2D<float> impulse = j * collisionEvent.normal;
    actorPhysicsComp->AddForce(-impulse);
    innerActorPhysicsComp->AddForce(impulse);
}

void PhysicsManager::moveActorsBackIntoLevel()
{
    for (auto actorPhysicsComponent : m_physicsComponentPtrMap)
    {
        std::shared_ptr<TransformComponent> actorTransformComponent = actorPhysicsComponent.second->GetTransformComponent();
        Vector2D<float> newPosition(actorTransformComponent->GetPosition());
        if (newPosition.x - actorTransformComponent->GetSize().x / 2 < 0)
        {
            newPosition.x = actorTransformComponent->GetSize().x / 2;
        }
        else if (newPosition.x + actorTransformComponent->GetSize().x / 2 > m_levelSize.x)
        {
            newPosition.x = m_levelSize.x - actorTransformComponent->GetSize().x / 2;
        }
        if (newPosition.y - actorTransformComponent->GetSize().y / 2 < 0)
        {
            newPosition.y = actorTransformComponent->GetSize().y / 2;
        }
        else if (newPosition.y + actorTransformComponent->GetSize().y / 2 > m_levelSize.y)
        {
            newPosition.y = m_levelSize.y - actorTransformComponent->GetSize().y / 2;
        }

        actorTransformComponent->SetPosition(newPosition);
    }
}