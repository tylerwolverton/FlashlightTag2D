#pragma once
#include "PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "Vector2D.h"
#include <vector>
#include <map>

class TransformComponent;

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

    void SetLevelSize(Vector2D<int> levelSize) { m_levelSize = levelSize; }

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
        float penetrationDepth;
        Vector2D<float> normal;

		CollisionEvent()
			: penetrationDepth(0),
			  normal(Vector2D<float>(0,0))
		{
		}

        CollisionEvent(float p_penetrationDepth, Vector2D<float> p_normal)
            : penetrationDepth(p_penetrationDepth),
              normal(p_normal)
        {
        }
    };

    CollisionEvent checkCircleCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent);
	CollisionEvent checkCircleBoxCollision(std::shared_ptr<TransformComponent> circleActorTransformComponent, std::shared_ptr<TransformComponent> boxActorTransformComponent);
	CollisionEvent checkBoxCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent);
	void resolvePenetration(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent);
    void resolveCollision(std::shared_ptr<PhysicsComponent> actorPhysicsComp, std::shared_ptr<PhysicsComponent> innerActorPhysicsComp, const PhysicsManager::CollisionEvent& collisionEvent);
    void moveActorsBackIntoLevel();

    // TODO: Cache changes
    //std::vector<PhysicsComponent> m_physicsComponentVec;
    //std::vector<std::shared_ptr<PhysicsComponent>> m_physicsComponentPtrVec;
    std::map<ComponentId, std::shared_ptr<PhysicsComponent>> m_physicsComponentPtrMap;

    Vector2D<int> m_levelSize;

    int m_lastComponentId;
    int getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };
};

