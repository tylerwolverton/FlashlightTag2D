#pragma once
#include "ActorComponent.h"
#include "Behavior.h"

class BaseLogicComponent :
	public ActorComponent
{
public:
	BaseLogicComponent();
	virtual ~BaseLogicComponent();

private:
	std::shared_ptr<Behavior> behavior;
};

