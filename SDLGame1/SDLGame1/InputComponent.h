#pragma once
#include "ActorComponent.h"

class InputComponent : public ActorComponent
{
public:
	InputComponent();
	virtual ~InputComponent();

	bool VInit() override;
	void VPostInit() override;
	void VUpdate(int deltaMs) override;

	ComponentId VGetComponentId() const override;
};

