#include "PhysicsManager.h"
#include "TransformComponent.h"
#include "Level.h"
#include "GameTile.h"
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
    m_environmentPhysicsComponentPtrVec.clear();
}

void PhysicsManager::LoadNewLevel(std::shared_ptr<Level> level)
{
    m_levelSize = level->GetLevelSize();

    for (auto tileVec : level->GetTileVec())
    {
        for (auto tile : tileVec)
        {
            auto physicsComp = tile->GetPhysicsComponent();
            if (physicsComp != nullptr)
            {
                m_environmentPhysicsComponentPtrVec.push_back(physicsComp);
            }
        }
    }
}

void PhysicsManager::Update(float deltaTime)
{
	ResolveCollisions(deltaTime);
    moveActorsBackIntoLevel();
}

void PhysicsManager::ResolveCollisions(float deltaTime)
{
    //for (auto actorPhysicsComponent : m_physicsComponentPtrVec)
    for (auto actorPhysicsComp : m_physicsComponentPtrMap)
    {
        std::shared_ptr<TransformComponent> actorTransformComp = actorPhysicsComp.second->GetTransformComponent();
		for (auto innerActorPhysicsComp : m_physicsComponentPtrMap)
		{
            std::shared_ptr<TransformComponent> innerTransformComp = innerActorPhysicsComp.second->GetTransformComponent();
			if (actorPhysicsComp.second->GetComponentId() != innerActorPhysicsComp.second->GetComponentId())
			{
                if (handleCollision(actorPhysicsComp.second, innerActorPhysicsComp.second, checkCircleCollision(actorTransformComp, innerTransformComp)))
                {
                    return;
                }
			}
		}
        for (auto environmentPhysicsComp : m_environmentPhysicsComponentPtrVec)
        {
            std::shared_ptr<TransformComponent> envTransformComp = environmentPhysicsComp->GetTransformComponent();
            CollisionEvent collisionEv = checkCircleBoxCollision(actorTransformComp, envTransformComp);
            handleCollision(actorPhysicsComp.second, environmentPhysicsComp, collisionEv);

            /*if (collisionEv.collisionDetected)
            {
                break;
            }*/
        }
	}
}

bool PhysicsManager::handleCollision(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, CollisionEvent collisionEvent)
{
    if (collisionEvent.collisionDetected)
    {
        resolvePenetration(actorPhysicsComp, innerActorPhysicsComp, collisionEvent);
        resolveCollision(actorPhysicsComp, innerActorPhysicsComp, collisionEvent);
        if (actorPhysicsComp->SignalCollision(innerActorPhysicsComp->GetParentActorId()))
        {
            return true;
        }
        if (innerActorPhysicsComp->SignalCollision(actorPhysicsComp->GetParentActorId()))
        {
            return true;
        }
    }

    return false;
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
    
    bool collisionDetected = false;
    if (dist.Length() - sizeSum < 0)
    {
        collisionDetected = true;
    }

    auto collisionEvent = CollisionEvent{ collisionDetected, dist.Length() - sizeSum, dist.Normalize() };

	return collisionEvent;
}

PhysicsManager::CollisionEvent PhysicsManager::checkCircleBoxCollision(std::shared_ptr<TransformComponent> circleActorTransformComponent, std::shared_ptr<TransformComponent> boxActorTransformComponent)
{
	Vector2D<float> boxPos = boxActorTransformComponent->GetPosition();
	Vector2D<float> boxSize = boxActorTransformComponent->GetSize();
	float boxMinX = boxPos.x - boxSize.x;
	float boxMaxX = boxPos.x + boxSize.x;
	float boxMinY = boxPos.y - boxSize.y;
	float boxMaxY = boxPos.y + boxSize.y;

    Vector2D<float> circlePos = circleActorTransformComponent->GetPosition();
    float circleRadius = circleActorTransformComponent->GetRadius();
    float circleMinX = circlePos.x - circleRadius;
    float circleMaxX = circlePos.x + circleRadius;
    float circleMinY = circlePos.y - circleRadius;
    float circleMaxY = circlePos.y + circleRadius;

	if (circleMaxX < boxMinX
		|| circleMinX > boxMaxX
		|| circleMaxY < boxMinY
		|| circleMinY > boxMaxY)
	{
		return CollisionEvent(false, -1, Vector2D<float>(0,0));
	}

    Vector2D<float> collisionPt;
    if (circleMaxX > boxMinX)
    {
        collisionPt.x = boxMinX;
    }
    else if (circleMinX < boxMaxX)
    { 
        collisionPt.x = boxMaxX;
    }

    if (circleMaxY > boxMinY)
    {
        collisionPt.y = boxMinY;
    }
    else if (circleMinY < boxMaxY)
    {
        collisionPt.y = boxMaxY;
    }

	Vector2D<float> dist = circlePos - collisionPt;
	auto collisionEvent = CollisionEvent{ true, dist.Length()/4, dist.Normalize() };

	return collisionEvent;
}

PhysicsManager::CollisionEvent PhysicsManager::checkBoxCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent)
{
	Vector2D<float> dist = actorTransformComponent->GetPosition() - innerActorTransformComponent->GetPosition();
	float sizeSum = actorTransformComponent->GetRadius() + innerActorTransformComponent->GetRadius();

	auto collisionEvent = CollisionEvent{ false, dist.Length() - sizeSum, dist.Normalize() };

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