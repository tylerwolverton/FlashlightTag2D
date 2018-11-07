#pragma once
#include "InputComponent.h"

class MainMenuInputComponent :
    public InputComponent
{
public:
    MainMenuInputComponent(ComponentId componentId);
    virtual ~MainMenuInputComponent();

    //void Update(GameActor& actor, float deltaMs) override;

private:
    int elapsedTicks;
    int maxTicks;
};

