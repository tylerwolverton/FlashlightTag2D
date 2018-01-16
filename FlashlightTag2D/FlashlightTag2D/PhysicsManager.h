#pragma once
#include "Types.h"
#include "PhysicsComponent.h"

#include <vector>

class TransformComponent;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(StrongGameActorPtrList gameActors, float deltaTime);
	void ResolveCollisions(StrongGameActorPtrList gameActors, float deltaTime);

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

    CollisionEvent checkCircleCollision(StrongGameActorPtr actor, StrongGameActorPtr innerActor);
	void resolvePenetration(StrongGameActorPtr actor, StrongGameActorPtr innerActor, const CollisionEvent& collisionEvent);
    void resolveCollision(StrongPhysicsComponentPtr actorPhysicsComp, StrongPhysicsComponentPtr innerActorPhysicsComp, const CollisionEvent& collisionEvent);
};

