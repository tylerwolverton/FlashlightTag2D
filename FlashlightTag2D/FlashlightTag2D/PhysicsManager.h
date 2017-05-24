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

	void Update(StrongGameActorPtrList gameActors);
	void ResolveCollisions(StrongGameActorPtrList gameActors);

	bool CheckCircleCollision(StrongGameActorPtr actor, StrongGameActorPtr innerActor);
	void MoveActors(std::shared_ptr<PhysicsComponent>& actorPhysicsComp, std::shared_ptr<PhysicsComponent>& innerActorPhysicsComp);
private:

};

