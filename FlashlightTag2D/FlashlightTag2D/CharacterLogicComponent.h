#pragma once
#include "LogicComponent.h"

class CharacterLogicComponent :
	public LogicComponent
{
public:
	CharacterLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent);
	virtual ~CharacterLogicComponent();

	void MoveUp() override;
	void MoveDown() override;
	void MoveRight() override;
	void MoveLeft() override;
};

