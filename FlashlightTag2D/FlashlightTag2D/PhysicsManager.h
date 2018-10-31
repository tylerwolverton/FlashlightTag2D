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

	void Update(float deltaTime);
	void ResolveCollisions(float deltaTime);

    void AddPhysicsComponentPtr(ComponentId compId, std::shared_ptr<PhysicsComponent> comp);
    void RemovePhysicsComponentPtr(ComponentId compId);
    void AddEnvironmentPhysicsComponentPtr(std::shared_ptr<PhysicsComponent> comp) { m_environmentPhysicsComponentPtrVec.push_back(comp); }

    void LoadNewLevel(std::shared_ptr<Level> level);
    //void SetLevelSize(Vector2D<int> levelSize) { m_levelSize = levelSize; }

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

        CollisionEvent(bool p_collisionDetected, float p_penetrationDepth, Vector2D<float> p_normal)
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

        CircleBoxCollisionEvent(bool p_collisionDetected, Vector2D<float> p_penetrationDepth, Vector2D<float> p_normal)
            : collisionDetected(p_collisionDetected),
              collisionLocation(p_penetrationDepth),
              normal(p_normal)
        {
        }
    };

    bool handleCollision(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, CollisionEvent collisionEvent);
    CollisionEvent checkCircleCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent);
    CircleBoxCollisionEvent checkCircleBoxCollision(std::shared_ptr<TransformComponent> circleActorTransformComponent, std::shared_ptr<TransformComponent> boxActorTransformComponent, std::shared_ptr<PhysicsComponent> actorPhysicsComp);
	CollisionEvent checkBoxCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent);
	void resolvePenetration(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent);
    void resolveCollision(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent);
    void moveActorsBackIntoLevel();

    // TODO: Cache changes
    //std::vector<PhysicsComponent> m_physicsComponentVec;
    //std::vector<std::shared_ptr<PhysicsComponent>> m_physicsComponentPtrVec;
    std::map<ComponentId, std::shared_ptr<PhysicsComponent>> m_physicsComponentPtrMap;
    std::vector<std::shared_ptr<PhysicsComponent>> m_environmentPhysicsComponentPtrVec;

    //std::shared_ptr<Level> m_curLevel;
    Vector2D<int> m_levelSize;

    int m_lastComponentId;
    int getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };
};

