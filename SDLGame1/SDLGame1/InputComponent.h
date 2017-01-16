#pragma once
#include "ActorComponent.h"

class InputComponent : public ActorComponent
{
public:
	InputComponent();
	virtual ~InputComponent();

	bool VInit(tinyxml2::XMLElement* pData) override;
	void VPostInit() override;
	void VUpdate(int deltaMs) override;

	ComponentId VGetComponentId() const override;
};

