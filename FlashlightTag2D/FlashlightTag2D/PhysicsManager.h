#pragma once
#include "Types.h"
#include "PhysicsComponent.h"
#include "Vector2D.h"
#include <vector>

class TransformComponent;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(StrongGameActorPtrList gameActors, float deltaTime);
	void ResolveCollisions(StrongGameActorPtrList gameActors, float deltaTime);

    void AddPhysicsComponent(PhysicsComponent comp);

private:
    struct CollisionEvent
    {
        float penetrationDepth;
        Vector2D<float> normal;

        CollisionEvent(float p_penetrationDepth, Vector2D<float> p_normal)
            : penetrationDepth(p_penetrationDepth),
              normal(p_normal)
        {
        }
    };

    CollisionEvent checkCircleCollision(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent);
	void resolvePenetration(std::shared_ptr<TransformComponent> actorTransformComponent, std::shared_ptr<TransformComponent> innerActorTransformComponent, const PhysicsManager::CollisionEvent& collisionEvent);
    void resolveCollision(PhysicsComponent& actorPhysicsComp, PhysicsComponent& innerActorPhysicsComp, const CollisionEvent& collisionEvent);
    
    std::vector<PhysicsComponent> m_physicsComponentVec;
};

