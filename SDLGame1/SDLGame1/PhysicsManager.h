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

	bool PhysicsManager::CircleCollision(StrongGameActorPtr actor1, StrongGameActorPtr actor2);

private:

};

