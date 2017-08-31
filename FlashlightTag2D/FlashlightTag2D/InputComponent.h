#pragma once
#include "ActorComponent.h"
class Command;

class InputComponent : public ActorComponent
{
public:
	InputComponent();
	virtual ~InputComponent();

	void Update(GameActor& actor, int deltaMs) override;

	ComponentId GetComponentId() const;
	virtual EComponentNames GetComponentName() const override;

private:
	std::shared_ptr<Command> m_pButtonEsc;
	std::shared_ptr<Command> m_pButtonW;
	std::shared_ptr<Command> m_pButtonA;
	std::shared_ptr<Command> m_pButtonS;
	std::shared_ptr<Command> m_pButtonD;
};

