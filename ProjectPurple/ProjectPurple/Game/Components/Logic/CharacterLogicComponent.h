#pragma once
#include "LogicComponent.h"

class CharacterLogicComponent :
    public LogicComponent
{
public:
    CharacterLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsCompPtr);
    virtual ~CharacterLogicComponent();

    void MoveUp() override;
    void MoveDown() override;
    void MoveRight() override;
    void MoveLeft() override;
    void Shoot() override;

private:
    std::shared_ptr<PhysicsComponent> m_physicsCompPtr; 

    uint32_t m_lastTickVal;
};
