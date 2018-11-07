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
    }

    for (auto actorPhysicsComp : m_physicsComponentPtrMap)
    {
        // Compare current pos with new pos and create cases to decide which order to go in. Break the loop and go the other way if necessary
        if (actorPhysicsComp.second->GetVelocity().x < 0)
        {
            if (checkEnvironmentCollisionBottomUp(actorPhysicsComp.second))
            {
                checkEnvironmentCollisionTopDown(actorPhysicsComp.second);
            }
        }
        else
        {
            if (checkEnvironmentCollisionTopDown(actorPhysicsComp.second))
            {
                checkEnvironmentCollisionBottomUp(actorPhysicsComp.second);
            }
        }
    }
}

//void PhysicsManager::ResolveCollisions(float deltaTime)
//{
//    //for (auto actorPhysicsComponent : m_physicsComponentPtrVec)
//    // check each actor for collision
//    for (auto actorPhysicsComp : m_physicsComponentPtrMap)
//    {
//        std::shared_ptr<GameActor> actor = actorPhysicsComp.second->GetParent();
//        std::shared_ptr<TransformComponent> actorTransformComp = actorPhysicsComp.second->GetTransformComponent();
//        // check each tile the actor is touching
//        for (auto tile : actor->GetTileVec())
//        {
//            // check collision agianst any actors also in the tile
//            for (auto innerActor : tile->GetActorMap())
//            {
//                if (actor->GetActorId() != innerActor.second->GetActorId())
//                {
//                    std::shared_ptr<TransformComponent> innerTransformComp = innerActor.second->GetTransformComponent();
//                    std::shared_ptr<PhysicsComponent> innerPhysicsComp = innerActor.second->GetPhysicsComponent();
//                    if (innerTransformComp == nullptr
//                        || innerPhysicsComp == nullptr)
//                    {
//                        continue;
//                    }
//                    if (handleCollision(actorPhysicsComp.second, innerPhysicsComp, checkCircleCollision(actorTransformComp, innerTransformComp)))
//                    {
//                        return;
//                    }
//                }
//            }
//
//            // check collision against tile itself
//            std::shared_ptr<PhysicsComponent> tilePhysicsComp = tile->GetPhysicsComponent();
//            if (tilePhysicsComp == nullptr)
//            {
//                continue;
//            }
//
//            std::shared_ptr<TransformComponent> tileTransformComp = tile->GetTransformComponent();
//            CollisionEvent collisionEv = checkCircleBoxCollision(actorTransformComp, tileTransformComp);
//            handleCollision(actorPhysicsComp.second, tilePhysicsComp, collisionEv);
//        }
//    }
//}

bool PhysicsManager::checkEnvironmentCollisionTopDown(std::shared_ptr<PhysicsComponent> actorPhysicsComp)
{
    std::shared_ptr<TransformComponent> actorTransformComp = actorPhysicsComp->GetTransformComponent();
    for (int i = 0; i < m_environmentPhysicsComponentPtrVec.size(); i++)
    {
        std::shared_ptr<TransformComponent> envTransformComp = m_environmentPhysicsComponentPtrVec[i]->GetTransformComponent();
        CircleBoxCollisionEvent collisionEv = checkCircleBoxCollision(actorTransformComp, envTransformComp, actorPhysicsComp);

        if (collisionEv.collisionDetected)
        {
            if (actorTransformComp->GetPosition().x > collisionEv.collisionLocation.x
                && actorTransformComp->GetPosition().y > collisionEv.collisionLocation.y
                || actorTransformComp->GetPosition().x < collisionEv.collisionLocation.x
                    && actorTransformComp->GetPosition().y < collisionEv.collisionLocation.y)
            {
                return true;
            }

            actorTransformComp->SetPosition(collisionEv.collisionLocation);
        }
    }

    return false;
}

bool PhysicsManager::checkEnvironmentCollisionBottomUp(std::shared_ptr<PhysicsComponent> actorPhysicsComp)
{
    std::shared_ptr<TransformComponent> actorTransformComp = actorPhysicsComp->GetTransformComponent();
    for (int i = m_environmentPhysicsComponentPtrVec.size() - 1; i >= 0; i--)
    {
        std::shared_ptr<TransformComponent> envTransformComp = m_environmentPhysicsComponentPtrVec[i]->GetTransformComponent();
        CircleBoxCollisionEvent collisionEv = checkCircleBoxCollision(actorTransformComp, envTransformComp, actorPhysicsComp);
        
        if (collisionEv.collisionDetected)
        {
            if (actorTransformComp->GetPosition().x < collisionEv.collisionLocation.x
                && actorTransformComp->GetPosition().y < collisionEv.collisionLocation.y)
            {
                return true;
            }

            actorTransformComp->SetPosition(collisionEv.collisionLocation);
        }
    }

    return false;
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

PhysicsManager::CircleBoxCollisionEvent PhysicsManager::checkCircleBoxCollision(std::shared_ptr<TransformComponent> circleActorTransformComponent, std::shared_ptr<TransformComponent> boxActorTransformComponent, std::shared_ptr<PhysicsComponent> actorPhysicsComp)
{
    // The y values for positions follow the traditional math convention of y increases as it moves up the screen
    // as opposed to the graphics convention where y increases as it moves down the screen

    // Box position is at the top left
    Vector2D<float> boxPos = boxActorTransformComponent->GetPosition();
    Vector2D<float> boxSize = boxActorTransformComponent->GetSize();
    float boxMinX = boxPos.x;
    float boxMaxX = boxPos.x + boxSize.x;
    float boxMinY = boxPos.y - boxSize.y;
    float boxMaxY = boxPos.y;

    // Circle position is in middle of circle
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
        return CircleBoxCollisionEvent(false, Vector2D<float>(0,0), Vector2D<float>(0,0));
    }

    Vector2D<float> lastCirclePos(circlePos - actorPhysicsComp->GetVelocity());
    Vector2D<float> collisionPt(Vector2D<float>(circlePos.x, circlePos.y));

    if (lastCirclePos.x + circleRadius < boxMinX && boxMinX < circleMaxX)
    {
        //circleActorTransformComponent->SetPosition(Vector2D<float>(boxMinX - circleRadius - .01f, circlePos.y - actorPhysicsComp->GetVelocity().y));
        //circleActorTransformComponent->SetPosition(Vector2D<float>(boxMinX - circleRadius - .01f, circlePos.y));
        //actorPhysicsComp->SetVelocity(Vector2D<float>(0, actorPhysicsComp->GetVelocity().y));
        collisionPt.x = boxMinX - circleRadius - .01f;
    }
    else if (lastCirclePos.x - circleRadius > boxMaxX && boxMaxX > circleMinX)
    {
        //circleActorTransformComponent->SetPosition(Vector2D<float>(boxMaxX + circleRadius + .01f, circlePos.y - actorPhysicsComp->GetVelocity().y));
        //circleActorTransformComponent->SetPosition(Vector2D<float>(boxMaxX + circleRadius + .01f, circlePos.y));
        //actorPhysicsComp->SetVelocity(Vector2D<float>(0, actorPhysicsComp->GetVelocity().y));
        collisionPt.x = boxMaxX + circleRadius + .01f;
    }

    // Check against bottom of box
    if (lastCirclePos.y + circleRadius < boxMinY && boxMinY < circleMaxY)
    //if (lastCirclePos.y + circleRadius < boxMaxY && boxMaxY < circleMinY)
    {
        //circleActorTransformComponent->SetPosition(Vector2D<float>(circleActorTransformComponent->GetPosition().x - actorPhysicsComp->GetVelocity().x, boxMinY - circleRadius - .01f));
        //circleActorTransformComponent->SetPosition(Vector2D<float>(circleActorTransformComponent->GetPosition().x, boxMinY - circleRadius - .01f));
        //actorPhysicsComp->SetVelocity(Vector2D<float>(actorPhysicsComp->GetVelocity().y, 0));
        collisionPt.y = boxMinY - circleRadius - .01f;
        //collisionPt.y = boxMaxY - circleRadius - .01f;
    }
    //else if (lastCirclePos.y - circleRadius > boxMinY && boxMinY > circleMaxY)
    else if (lastCirclePos.y - circleRadius > boxMaxY && boxMaxY > circleMinY)
    {
        //circleActorTransformComponent->SetPosition(Vector2D<float>(circleActorTransformComponent->GetPosition().x - actorPhysicsComp->GetVelocity().x, boxMaxY + circleRadius + .01f));
        //circleActorTransformComponent->SetPosition(Vector2D<float>(circleActorTransformComponent->GetPosition().x, boxMaxY + circleRadius + .01f));
        //actorPhysicsComp->SetVelocity(Vector2D<float>(actorPhysicsComp->GetVelocity().y, 0));
        collisionPt.y = boxMaxY + circleRadius + .01f;
        //collisionPt.y = boxMinY + circleRadius + .01f;
    }
    //circleActorTransformComponent->SetPosition(collisionPt);
    //actorPhysicsComp->SetVelocity(Vector2D<float>(0,0));

    //Vector2D<float> collisionPt(circlePos);
    //if (abs(actorPhysicsComp->GetVelocity().x) > abs(actorPhysicsComp->GetVelocity().y))
    //if (abs(actorPhysicsComp->GetVelocity().x) > 0)
    //{
    //    if (circlePos.x > boxPos.x
    //        && circleMinX < boxMaxX)
    //    {
    //        //collisionPt.x = boxMaxX + circleRadius + .01f;
    //        //collisionPt.x = circleActorTransformComponent->GetPosition().x - actorPhysicsComp->GetVelocity().x;
    //        //circleActorTransformComponent->SetPosition circleActorTransformComponent->GetPosition() - actorPhysicsComp->GetVelocity()
    //    }

    //    else if (circlePos.x < boxPos.x
    //        &&  circleMaxX > boxMinX)
    //    {
    //        //collisionPt.x = boxMinX - circleRadius - .01f;
    //        //collisionPt.x = circleActorTransformComponent->GetPosition().x - actorPhysicsComp->GetVelocity().x;
    //    }
    //}
    //
    //if(abs(actorPhysicsComp->GetVelocity().y) > 0)
    //{
    //    if (circleMaxY > boxMinY || circleMinY < boxMaxY)
    //    {
    //        //collisionPt.y = boxMinY - circleRadius - .01f;
    //        collisionPt.y = circleActorTransformComponent->GetPosition().y - actorPhysicsComp->GetVelocity().y;
    //    }
    //    //if (circlePos.y < boxPos.y
    //    //    &&  circleMaxY > boxMinY)
    //    //{
    //    //    //collisionPt.y = boxMinY - circleRadius - .01f;
    //    //    collisionPt.y = circleActorTransformComponent->GetPosition().y - actorPhysicsComp->GetVelocity().y;
    //    //}

    //    //else if (circlePos.y > boxPos.y
    //    //    &&  circleMinY < boxMaxY)
    //    //{
    //    //    //collisionPt.y = boxMaxY + circleRadius + .01f;
    //    //    collisionPt.y = circleActorTransformComponent->GetPosition().y - actorPhysicsComp->GetVelocity().y;
    //    //}
    //}

    Vector2D<float> dist = circlePos - boxPos;
    //circleActorTransformComponent->SetPosition(circleActorTransformComponent->GetPosition() - actorPhysicsComp->GetVelocity());
    //auto collisionEvent = CircleBoxCollisionEvent{ true, circleActorTransformComponent->GetPosition() - actorPhysicsComp->GetVelocity(), dist.Normalize() };
    auto collisionEvent = CircleBoxCollisionEvent{ true, collisionPt, dist.Normalize() };

    auto gameStateComp = circleActorTransformComponent->GetParent()->GetGameStateComponent();
    if (gameStateComp != nullptr
        && gameStateComp->GetName() == "Projectile")
    {
        auto lifeComp = circleActorTransformComponent->GetParent()->GetLifeComponent();
        if (lifeComp != nullptr)
        {
            lifeComp->Die();
        }
    }

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
        if (newPosition.y - actorTransformComponent->GetSize().y / 2 < Level::TILE_HEIGHT * 2)
        {
            newPosition.y = actorTransformComponent->GetSize().y / 2 + Level::TILE_HEIGHT * 2;
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