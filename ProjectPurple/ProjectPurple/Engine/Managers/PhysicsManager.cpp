#include "PhysicsManager.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "LifeComponent.h"
#include "Level.h"
#include "GameTile.h"
#include "GameActor.h"
#include <stdlib.h>
#include <algorithm>

PhysicsManager::PhysicsManager()
    : m_lastComponentId(0),
      m_levelSize(Vector2D<int>(-1, -1))
{
}

PhysicsManager::~PhysicsManager()
{
	ClearPhysicsComponents();
}

void PhysicsManager::ClearPhysicsComponents()
{
    m_physicsComponentPtrMap.clear();
    m_environmentPhysicsComponentPtrVec.clear();
}

void PhysicsManager::LoadNewLevel(const std::shared_ptr<Level>& level)
{
    m_levelSize = level->GetLevelSize();
    auto tilePtrVec = level->GetTileVec();
    if (tilePtrVec != nullptr)
    {
        for (auto tilePtrVec : (*tilePtrVec))
        {
            for (auto tilePtr : tilePtrVec)
            {
                auto physicsCompPtr = tilePtr->GetPhysicsCompPtr();
                if (physicsCompPtr != nullptr)
                {
                    m_environmentPhysicsComponentPtrVec.push_back(physicsCompPtr);
                }
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
    for (auto actorPhysicsCompEntry : m_physicsComponentPtrMap)
    {
        auto actorTransformCompPtr = actorPhysicsCompEntry.second->GetTransformCompPtr();
        for (auto innerActorPhysicsCompEntry : m_physicsComponentPtrMap)
        {
            auto innerTransformCompPtr = innerActorPhysicsCompEntry.second->GetTransformCompPtr();
            if (actorPhysicsCompEntry.second->GetComponentId() != innerActorPhysicsCompEntry.second->GetComponentId())
            {
                if (handleCollision(actorPhysicsCompEntry.second, innerActorPhysicsCompEntry.second, checkCircleCollision(actorTransformCompPtr, innerTransformCompPtr)))
                {
                    return;
                }
            }
        }
    }

    for (auto actorPhysicsCompEntry : m_physicsComponentPtrMap)
    {
        // Compare current pos with new pos and create cases to decide which order to go in. Break the loop and go the other way if necessary
        if (actorPhysicsCompEntry.second->GetVelocity().x < 0)
        {
            if (checkEnvironmentCollisionBottomUp(actorPhysicsCompEntry.second))
            {
                checkEnvironmentCollisionTopDown(actorPhysicsCompEntry.second);
            }
        }
        else
        {
            if (checkEnvironmentCollisionTopDown(actorPhysicsCompEntry.second))
            {
                checkEnvironmentCollisionBottomUp(actorPhysicsCompEntry.second);
            }
        }
    }
}

void PhysicsManager::AddPhysicsComponentPtr(ComponentId compId, const std::shared_ptr<PhysicsComponent>& physicsCompPtr)
{
	m_physicsComponentPtrMap.insert(std::make_pair(compId, physicsCompPtr));
}

void PhysicsManager::RemovePhysicsComponentPtr(ComponentId compId)
{
	m_physicsComponentPtrMap.erase(compId);
}

bool PhysicsManager::handleCollision(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr, 
                                     const std::shared_ptr<PhysicsComponent>& innerActorPhysicsCompPtr, 
                                     const CollisionEvent& collisionEvent)
{
    if (collisionEvent.collisionDetected)
    {
        resolvePenetration(actorPhysicsCompPtr, innerActorPhysicsCompPtr, collisionEvent);
        resolveCollision(actorPhysicsCompPtr, innerActorPhysicsCompPtr, collisionEvent);
        if (actorPhysicsCompPtr->SignalCollision(innerActorPhysicsCompPtr->GetParentActorId()))
        {
            return true;
        }
        if (innerActorPhysicsCompPtr->SignalCollision(actorPhysicsCompPtr->GetParentActorId()))
        {
            return true;
        }
    }

    return false;
}

bool PhysicsManager::checkEnvironmentCollisionTopDown(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr)
{
	auto actorTransformCompPtr = actorPhysicsCompPtr->GetTransformCompPtr();
	for (int i = 0; i < m_environmentPhysicsComponentPtrVec.size(); i++)
	{
		auto envTransformCompPtr = m_environmentPhysicsComponentPtrVec[i]->GetTransformCompPtr();
		CircleBoxCollisionEvent collisionEv = checkCircleBoxCollision(actorTransformCompPtr, envTransformCompPtr, actorPhysicsCompPtr);

		if (collisionEv.collisionDetected)
		{
			if (actorTransformCompPtr->GetPosition().x > collisionEv.collisionLocation.x
				&& actorTransformCompPtr->GetPosition().y > collisionEv.collisionLocation.y
				|| actorTransformCompPtr->GetPosition().x < collisionEv.collisionLocation.x
				&& actorTransformCompPtr->GetPosition().y < collisionEv.collisionLocation.y)
			{
				return true;
			}

			actorTransformCompPtr->SetPosition(collisionEv.collisionLocation);
		}
	}

	return false;
}

bool PhysicsManager::checkEnvironmentCollisionBottomUp(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr)
{
	auto actorTransformCompPtr = actorPhysicsCompPtr->GetTransformCompPtr();
	for (int i = m_environmentPhysicsComponentPtrVec.size() - 1; i >= 0; i--)
	{
		auto envTransformCompPtr = m_environmentPhysicsComponentPtrVec[i]->GetTransformCompPtr();
		CircleBoxCollisionEvent collisionEv = checkCircleBoxCollision(actorTransformCompPtr, envTransformCompPtr, actorPhysicsCompPtr);

		if (collisionEv.collisionDetected)
		{
			if (actorTransformCompPtr->GetPosition().x < collisionEv.collisionLocation.x
				&& actorTransformCompPtr->GetPosition().y < collisionEv.collisionLocation.y)
			{
				return true;
			}

			actorTransformCompPtr->SetPosition(collisionEv.collisionLocation);
		}
	}

	return false;
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

PhysicsManager::CollisionEvent PhysicsManager::checkCircleCollision(const std::shared_ptr<TransformComponent>& actorTransformCompPtr,
                                                                    const std::shared_ptr<TransformComponent>& innerActorTransformCompPtr)
{
    Vector2D<float> distBetweenActors = actorTransformCompPtr->GetPosition() - innerActorTransformCompPtr->GetPosition();
    float sumOfSizes = actorTransformCompPtr->GetRadius() + innerActorTransformCompPtr->GetRadius();
    
    bool collisionDetected = false;
    if (distBetweenActors.Length() - sumOfSizes < 0)
    {
        collisionDetected = true;
    }

    return CollisionEvent{ collisionDetected, distBetweenActors.Length() - sumOfSizes, distBetweenActors.Normalize() };
}

PhysicsManager::CircleBoxCollisionEvent PhysicsManager::checkCircleBoxCollision(const std::shared_ptr<TransformComponent>& circleActorTransformCompPtr,
                                                                                const std::shared_ptr<TransformComponent>& boxActorTransformCompPtr, 
                                                                                const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr)
{
    // The y values for positions follow the traditional math convention of y increases as it moves up the screen
    // as opposed to the graphics convention where y increases as it moves down the screen

    // Box position is at the top left
    Vector2D<float> boxPos = boxActorTransformCompPtr->GetPosition();
    Vector2D<float> boxSize = boxActorTransformCompPtr->GetSize();
    float boxMinX = boxPos.x;
    float boxMaxX = boxPos.x + boxSize.x;
    float boxMinY = boxPos.y - boxSize.y;
    float boxMaxY = boxPos.y;

    // Circle position is in middle of circle
    Vector2D<float> circlePos = circleActorTransformCompPtr->GetPosition();
    float circleRadius = circleActorTransformCompPtr->GetRadius();
    float circleMinX = circlePos.x - circleRadius;
    float circleMaxX = circlePos.x + circleRadius;
    float circleMinY = circlePos.y - circleRadius;
    float circleMaxY = circlePos.y + circleRadius;

    if (circleMaxX < boxMinX
        || circleMinX > boxMaxX
        || circleMaxY < boxMinY
        || circleMinY > boxMaxY)
    {
        return CircleBoxCollisionEvent(false, Vector2D<float>(0,0), Vector2D<float>(0,0));
    }

    Vector2D<float> lastCirclePos(circlePos - actorPhysicsCompPtr->GetVelocity());
    Vector2D<float> collisionPt(Vector2D<float>(circlePos.x, circlePos.y));

    if (lastCirclePos.x + circleRadius < boxMinX && boxMinX < circleMaxX)
    {
        collisionPt.x = boxMinX - circleRadius - .01f;
    }
    else if (lastCirclePos.x - circleRadius > boxMaxX && boxMaxX > circleMinX)
    {
        collisionPt.x = boxMaxX + circleRadius + .01f;
    }

    // Check against bottom of box
    if (lastCirclePos.y + circleRadius < boxMinY && boxMinY < circleMaxY)
    {
        collisionPt.y = boxMinY - circleRadius - .01f;
    }
    else if (lastCirclePos.y - circleRadius > boxMaxY && boxMaxY > circleMinY)
    {
        collisionPt.y = boxMaxY + circleRadius + .01f;
    }

    Vector2D<float> distBetweenActors = circlePos - boxPos;
    auto collisionEvent = CircleBoxCollisionEvent{ true, collisionPt, distBetweenActors.Normalize() };

    // If a projectile is hitting an environment actor, destroy the projectile
    auto gameStateCompPtr = circleActorTransformCompPtr->GetParent()->GetGameStateCompPtr();
    if (gameStateCompPtr != nullptr
        && gameStateCompPtr->GetName() == "Projectile")
    {
        auto lifeCompPtr = circleActorTransformCompPtr->GetParent()->GetLifeCompPtr();
        if (lifeCompPtr != nullptr)
        {
            lifeCompPtr->Die();
        }
    }

    return collisionEvent;
}

PhysicsManager::CollisionEvent PhysicsManager::checkBoxCollision(const std::shared_ptr<TransformComponent>& actorTransformCompPtr, 
                                                                 const std::shared_ptr<TransformComponent>& innerActorTransformCompPtr)
{
    Vector2D<float> distBetweenActors = actorTransformCompPtr->GetPosition() - innerActorTransformCompPtr->GetPosition();
    float sizeSum = actorTransformCompPtr->GetRadius() + innerActorTransformCompPtr->GetRadius();

    auto collisionEvent = CollisionEvent{ false, distBetweenActors.Length() - sizeSum, distBetweenActors.Normalize() };

    return collisionEvent;
}

void PhysicsManager::resolvePenetration(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr, 
                                        const std::shared_ptr<PhysicsComponent>& innerActorPhysicsCompPtr, 
                                        const PhysicsManager::CollisionEvent& collisionEvent)
{
	// Resolve penetration of actors in proportion to each actor's mass
    float combinedMass = actorPhysicsCompPtr->GetMass() + innerActorPhysicsCompPtr->GetMass();
    float totalMoveDist = abs(collisionEvent.penetrationDepth) / combinedMass;
    
    actorPhysicsCompPtr->GetTransformCompPtr()->SetPosition(actorPhysicsCompPtr->GetTransformCompPtr()->GetPosition() + collisionEvent.normal * totalMoveDist * innerActorPhysicsCompPtr->GetMass());
    innerActorPhysicsCompPtr->GetTransformCompPtr()->SetPosition(innerActorPhysicsCompPtr->GetTransformCompPtr()->GetPosition() - collisionEvent.normal * totalMoveDist * actorPhysicsCompPtr->GetMass());
}

void PhysicsManager::resolveCollision(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr, 
                                      const std::shared_ptr<PhysicsComponent>& innerActorPhysicsCompPtr, 
                                      const PhysicsManager::CollisionEvent& collisionEvent)
{
    Vector2D<float> relativeVelocity = innerActorPhysicsCompPtr->GetVelocity() - actorPhysicsCompPtr->GetVelocity();

    float velAlongNormal = relativeVelocity.Dot(collisionEvent.normal);

    // Do not resolve if velocities are separating
//    if (velAlongNormal > 0)
 //       return;

    // Calculate restitution
    float e = std::min(actorPhysicsCompPtr->GetRestitution(), innerActorPhysicsCompPtr->GetRestitution());

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= 1 / actorPhysicsCompPtr->GetMass() + 1 / innerActorPhysicsCompPtr->GetMass();

    // Apply impulse
    Vector2D<float> impulse = j * collisionEvent.normal;
    actorPhysicsCompPtr->AddForce(-impulse);
    innerActorPhysicsCompPtr->AddForce(impulse);
}

void PhysicsManager::moveActorsBackIntoLevel()
{
    // Check for actors outside the level boundaries and reset them to the edge of the level
    for (auto actorPhysicsCompEntry : m_physicsComponentPtrMap)
    {
        auto actorTransformCompPtr = actorPhysicsCompEntry.second->GetTransformCompPtr();
        Vector2D<float> newPosition(actorTransformCompPtr->GetPosition());
        if (newPosition.x - actorTransformCompPtr->GetSize().x / 2 < 0)
        {
            newPosition.x = actorTransformCompPtr->GetSize().x / 2;
        }
        else if (newPosition.x + actorTransformCompPtr->GetSize().x / 2 > m_levelSize.x)
        {
            newPosition.x = m_levelSize.x - actorTransformCompPtr->GetSize().x / 2;
        }
        if (newPosition.y - actorTransformCompPtr->GetSize().y / 2 < Level::TILE_HEIGHT * 2)
        {
            newPosition.y = actorTransformCompPtr->GetSize().y / 2 + Level::TILE_HEIGHT * 2;
        }
        else if (newPosition.y + actorTransformCompPtr->GetSize().y / 2 > m_levelSize.y)
        {
            newPosition.y = m_levelSize.y - actorTransformCompPtr->GetSize().y / 2;
        }

        Vector2D<float> newVelocity(actorPhysicsCompEntry.second->GetVelocity());
        if (actorTransformCompPtr->GetPosition().x != newPosition.x)
        {
            newVelocity.x = 0;
        }
        if (actorTransformCompPtr->GetPosition().y != newPosition.y)
        {
            newVelocity.y = 0;
        }

        actorTransformCompPtr->SetPosition(newPosition);
        actorPhysicsCompEntry.second->SetVelocity(newVelocity);
    }
}