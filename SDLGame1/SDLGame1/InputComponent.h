#pragma once
#include "ActorComponent.h"
class Command;

class InputComponent : public ActorComponent
{
public:
	InputComponent();
	virtual ~InputComponent();

	bool Init(tinyxml2::XMLElement* pData) override;
	void PostInit() override;
	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;
	virtual EComponentNames GetComponentName() const override;

private:
	std::shared_ptr<Command> buttonEsc;
	std::shared_ptr<Command> buttonW;
	std::shared_ptr<Command> buttonA;
	std::shared_ptr<Command> buttonS;
	std::shared_ptr<Command> buttonD;
};

