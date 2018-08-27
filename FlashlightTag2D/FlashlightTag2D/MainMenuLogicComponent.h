#pragma once
#include "LogicComponent.h"

class MainMenuLogicComponent :
	public LogicComponent
{
public:
	MainMenuLogicComponent(ComponentId componentId);
	virtual ~MainMenuLogicComponent();

	void MoveUp() override;
	void MoveDown() override;
	void StartGame() override;
};

