#pragma once
#include "Actor.h"
class GameActor :
	public Actor
{
public:
	GameActor(ActorId id);
	virtual ~GameActor();

	virtual void Update(int delatMs);

	virtual void MoveUp() {};
};

