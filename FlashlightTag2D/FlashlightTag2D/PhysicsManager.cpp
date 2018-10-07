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

				CollisionEvent collisionEvent;
				/*if (actorTransformComponent->GetSize().x == actorTransformComponent->GetSize().y
					&& innerTransformComponent->GetSize().x == innerTransformComponent->GetSize().y)
				{*/
					collisionEvent = checkCircleCollision(actorTransformComponent, innerTransformComponent);
				/*}
				else if (actorTransformComponent->GetSize().x == actorTransformComponent->GetSize().y
					&& innerTransformComponent->GetSize().x == innerTransformComponent->GetSize().y)
				{
					collisionEvent = checkCircleCollision(actorTransformComponent, innerTransformComponent);
				}
				else if(actorTransformComponent->GetSize().x == actorTransformComponent->GetSize().y
					&& innerTransformComponent->GetSize().x == innerTransformComponent->GetSize().y)
				{
					collisionEvent = checkCircleCollision(actorTransformComponent, innerTransformComponent);
				}
				else
				{

				}*/

				if (collisionEvent.penetrationDepth < 0)
				{
                    resolvePenetration(actorPhysicsComponent.second, innerActorPhysicsComponent.second, collisionEvent);
                    resolveCollision(actorPhysicsComponent.second, innerActorPhysicsComponent.second, collisionEvent);
                    if (actorPhysicsComponent.second->SignalCollision(innerActorPhysicsComponent.second->GetParentActorId()))
                    {
                        return;
                    }
                    if(innerActorPhysicsComponent.second->SignalCollision(actorPhysicsComponent.second->GetParentActorId()))
                    {
                        return;
                    }
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

PhysicsManager::CollisionEvent PhysicsManager::checkCircleBoxCollision(std::shared_ptr<TransformComponent> circleActorTransformComponent, std::shared_ptr<TransformComponent> boxActorTransformComponent)
{
	Vector2D<float> boxPos = boxActorTransformComponent->GetPosition();
	Vector2D<float> boxSize = boxActorTransformComponent->GetSize();
	/*Vector2D<float> boxTopLeft = boxPos + Vector2D<float>(-boxSize.x, -boxSize.y);
	Vector2D<float> boxTopRight = boxPos + Vector2D<float>(boxSize.x, -boxSize.y);
	Vector2D<float> boxBotLeft = boxPos + Vector2D<float>(-boxSize.x, boxSize.y);
	Vector2D<float> boxBotRight = boxPos + Vector2D<float>(boxSize.x, boxSize.y);*/

	float xMin = boxPos.x - boxSize.x;
	float xMax = boxPos.x + boxSize.x;
	float yMin = boxPos.y + boxSize.y;
	float yMax = boxPos.y - boxSize.y;

	Vector2D<float> circlePos = circleActorTransformComponent->GetPosition();
	float circleRadius = circleActorTransformComponent->GetRadius();
	if (circlePos.x + circleRadius < xMin 
		|| circlePos.x - circleRadius > xMax
		|| circlePos.y - circleRadius < yMin
		|| circlePos.y + circleRadius > yMax)
	{
		return CollisionEvent(-1, Vector2D<float>(0,0));
	}

	Vector2D<float> dist = boxActorTransformComponent->GetPosition() - circleActorTransformComponent->GetPosition();
	auto collisionEvent = CollisionEvent{ dist.Length(), dist.Normalize() };

	return collisionEvent;
}

PhysicsManager::CollisionEvent PhysicsManager::checkBoxCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent)
{
	Vector2D<float> dist = actorTransformComponent->GetPosition() - innerActorTransformComponent->GetPosition();
	float sizeSum = actorTransformComponent->GetRadius() + innerActorTransformComponent->GetRadius();

	auto collisionEvent = CollisionEvent{ dist.Length() - sizeSum, dist.Normalize() };

	return collisionEvent;
}

void PhysicsManager::resolvePenetration(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent)
{
	float combinedMass = actorPhysicsComp->GetMass() + innerActorPhysicsComp->GetMass();
    float totalMoveDist = abs(collisionEvent.penetrationDepth) / combinedMass;
	
	actorPhysicsComp->GetTransformComponent()->SetPosition(actorPhysicsComp->GetTransformComponent()->GetPosition() + collisionEvent.normal * totalMoveDist * innerActorPhysicsComp->GetMass());
	innerActorPhysicsComp->GetTransformComponent()->SetPosition(innerActorPhysicsComp->GetTransformComponent()->GetPosition() - collisionEvent.normal * totalMoveDist * actorPhysicsComp->GetMass());
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

		Vector2D<float> newVelocity(actorPhysicsComponent.second->GetVelocity());
		if (actorTransformComponent->GetPosition().x != newPosition.x)
		{
			newVelocity.x = 0;
		}
		if (actorTransformComponent->GetPosition().y != newPosition.y)
		{
			newVelocity.y = 0;
		}

        actorTransformComponent->SetPosition(newPosition);
		actorPhysicsComponent.second->SetVelocity(newVelocity);
    }
}