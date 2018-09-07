#pragma once
#include "ActorComponent.h"

class Command;

class InputComponent : public ActorComponent
{
public:
	InputComponent(ComponentId componentId);
	virtual ~InputComponent();

	virtual void Update(GameActor& actor, float deltaMs) override;

	virtual const EComponentNames GetComponentName() const override;

protected:
	std::shared_ptr<Command> m_pButtonW;
	std::shared_ptr<Command> m_pButtonA;
	std::shared_ptr<Command> m_pButtonS;
	std::shared_ptr<Command> m_pButtonD;
	std::shared_ptr<Command> m_pButtonEsc;
	std::shared_ptr<Command> m_pButtonSpace;
	std::shared_ptr<Command> m_pButtonReturn;
};

