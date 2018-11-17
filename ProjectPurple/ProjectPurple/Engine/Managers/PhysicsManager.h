#pragma once
#include "PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "Vector2D.h"
#include <vector>
#include <map>

class TransformComponent;
class Level;

class PhysicsManager
{
public:
    PhysicsManager();
    ~PhysicsManager();

    void ClearPhysicsComponents();
	void LoadNewLevel(const std::shared_ptr<Level>& level);

    void Update(float deltaTime);
    void ResolveCollisions(float deltaTime);

    void AddPhysicsComponentPtr(ComponentId compId, const std::shared_ptr<PhysicsComponent>& physicsCompPtr);
    void RemovePhysicsComponentPtr(ComponentId compId);
    void AddEnvironmentPhysicsComponentPtr(const std::shared_ptr<PhysicsComponent>& physicsCompPtr) 
    { 
        m_environmentPhysicsComponentPtrVec.push_back(physicsCompPtr); 
    }

    // TODO: Cache changes
    //void AddPhysicsComponent(PhysicsComponent comp);
    /*int AddPlayerPhysicsComponent(std::shared_ptr<TransformComponent> transformComponent,
                                  float maxSpeed,
                                  float mass,
                                  float restitution,
                                  Vector2D<float> velocity = Vector2D<float>(0, 0),
                                  Vector2D<float> acceleration = Vector2D<float>(0, 0));*/

private:
    struct CollisionEvent
    {
        bool collisionDetected;
        float penetrationDepth;
        Vector2D<float> normal;

        CollisionEvent()
            : collisionDetected(false),
              penetrationDepth(0),
              normal(Vector2D<float>(0,0))
        {
        }

        CollisionEvent(bool p_collisionDetected, float p_penetrationDepth, const Vector2D<float>& p_normal)
            : collisionDetected(p_collisionDetected),
              penetrationDepth(p_penetrationDepth),
              normal(p_normal)
        {
        }
    };

    struct CircleBoxCollisionEvent
    {
        bool collisionDetected;
        Vector2D<float> collisionLocation;
        Vector2D<float> normal;

        CircleBoxCollisionEvent()
            : collisionDetected(false),
              collisionLocation(Vector2D<float>(0, 0)),
              normal(Vector2D<float>(0, 0))
        {
        }

        CircleBoxCollisionEvent(bool p_collisionDetected, const Vector2D<float>& p_collisionLocation, const Vector2D<float>& p_normal)
            : collisionDetected(p_collisionDetected),
              collisionLocation(p_collisionLocation),
              normal(p_normal)
        {
        }
    };
	
    // TODO: Cache changes
    //std::vector<PhysicsComponent> m_physicsComponentVec;
    //std::vector<std::shared_ptr<PhysicsComponent>> m_physicsComponentPtrVec;
    std::map<ComponentId, std::shared_ptr<PhysicsComponent>> m_physicsComponentPtrMap;
    std::vector<std::shared_ptr<PhysicsComponent>> m_environmentPhysicsComponentPtrVec;

    Vector2D<int> m_levelSize;

    int m_lastComponentId;
    int getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };

	bool handleCollision(const std::shared_ptr<PhysicsComponent>& actorPhysicsComp, 
                         const std::shared_ptr<PhysicsComponent>& innerActorPhysicsCompPtr, 
                         const CollisionEvent& collisionEvent);

	bool checkEnvironmentCollisionTopDown(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr);

	bool checkEnvironmentCollisionBottomUp(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr);

	CollisionEvent checkCircleCollision(const std::shared_ptr<TransformComponent>& actorTransformCompPtr, 
                                        const std::shared_ptr<TransformComponent>& innerActorTransformCompPtr);

	CircleBoxCollisionEvent checkCircleBoxCollision(const std::shared_ptr<TransformComponent>& circleActorTransformCompPtr, 
                                                    const std::shared_ptr<TransformComponent>& boxActorTransformCompPtr, 
                                                    const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr);

	CollisionEvent checkBoxCollision(const std::shared_ptr<TransformComponent>& actorTransformCompPtr, 
                                     const std::shared_ptr<TransformComponent>& innerActorTransformCompPtr);

	void resolvePenetration(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr, 
                            const std::shared_ptr<PhysicsComponent>& innerActorPhysicsCompPtr, 
                            const CollisionEvent& collisionEvent);
	
    void resolveCollision(const std::shared_ptr<PhysicsComponent>& actorPhysicsCompPtr, 
                          const std::shared_ptr<PhysicsComponent>& innerActorPhysicsCompPtr, 
                          const CollisionEvent& collisionEvent);
	
    void moveActorsBackIntoLevel();
};

