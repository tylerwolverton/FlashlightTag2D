#pragma once
#include "Level.h"
class Level1 :
	public Level
{
public:
	Level1();
	virtual ~Level1();

	void RunShader() override;
};

