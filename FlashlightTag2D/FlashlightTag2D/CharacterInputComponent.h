#pragma once
#include "InputComponent.h"

class CharacterInputComponent :
	public InputComponent
{
public:
	CharacterInputComponent(ComponentId componentId);
	virtual ~CharacterInputComponent();
};

