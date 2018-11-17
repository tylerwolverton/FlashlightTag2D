#pragma once
#include <cstdint>

#include "LogicComponent.h"

class TransformComponent;

class MainMenuLogicComponent :
    public LogicComponent
{
public:
    MainMenuLogicComponent(ComponentId componentId, 
                           const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>>& buttonTransformCompMapPtr);
    virtual ~MainMenuLogicComponent();

    void MoveUp() override;
    void MoveDown() override;
    void Select() override;

private:
    enum EMenuButtons
    {
        Play,
        Exit
    };

    int m_selectedButtonIdx;
    std::vector<EMenuButtons> m_buttonVec;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>> m_buttonTransformCompPtrMapPtr;

    uint32_t m_lastTickVal;
};

