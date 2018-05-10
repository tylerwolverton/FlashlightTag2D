#pragma once
#include "Behavior.h"

#include <vector>

class HideBehavior :
	public Behavior
{
public:
	HideBehavior();
	virtual ~HideBehavior();

	virtual std::vector<std::shared_ptr<Command>> Update(const GameActor& thisActor) override;

private:
    std::vector<std::shared_ptr<Command>> RunFromSeekers(const std::shared_ptr<TransformComponent> thisActorTransformComponent, const std::vector<std::shared_ptr<TransformComponent>>& seekers);
};

