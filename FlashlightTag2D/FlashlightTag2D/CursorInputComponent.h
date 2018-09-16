#pragma once
#include "InputComponent.h"

class CursorInputComponent :
	public InputComponent
{
public:
	CursorInputComponent(ComponentId componentId);
	virtual ~CursorInputComponent();
};

