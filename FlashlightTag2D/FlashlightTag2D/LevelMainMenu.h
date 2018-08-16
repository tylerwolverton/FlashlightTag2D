#pragma once
#include "Level.h"
class LevelMainMenu :
	public Level
{
public:
	LevelMainMenu();
	virtual ~LevelMainMenu();

	void RunShader() override;
};

