#pragma once
#include "Behavior.h"
#include "Types.h"

#include <vector>

class HideBehavior :
	public Behavior
{
public:
	HideBehavior();
	virtual ~HideBehavior();

	virtual CommandList Update(const GameActor& thisActor) override;

private:
    CommandList RunFromSeekers(const StrongTransformComponentPtr thisActorTransformComponent, const std::vector<StrongTransformComponentPtr>& seekers);
};

