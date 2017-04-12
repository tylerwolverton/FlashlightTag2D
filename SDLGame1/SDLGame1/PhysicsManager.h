#pragma once
#include "Types.h"

#include <vector>

class TransformComponent;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(StrongGameActorPtrList gameActors);
	void ResolveCollisions(StrongGameActorPtrList gameActors);

	bool PhysicsManager::CheckCircleCollision(StrongGameActorPtr actor, StrongGameActorPtr innerActor);

private:

};

