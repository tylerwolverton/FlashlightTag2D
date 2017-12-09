#pragma once
#include "Behavior.h"
#include "Types.h"

class HideBehavior :
	public Behavior
{
public:
	HideBehavior();
	virtual ~HideBehavior();

	virtual CommandList Update(GameActor& thisActor) override;

};

