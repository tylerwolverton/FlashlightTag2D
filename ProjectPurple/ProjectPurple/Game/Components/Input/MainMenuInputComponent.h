#pragma once
#include "InputComponent.h"

class MainMenuInputComponent :
    public InputComponent
{
public:
    MainMenuInputComponent(ComponentId componentId);
    virtual ~MainMenuInputComponent();

private:
    int elapsedTicks;
    int maxTicks;
};

