#pragma once
#include <cstdint>

#include "LogicComponent.h"

class TransformComponent;

class MainMenuLogicComponent :
    public LogicComponent
{
public:
    MainMenuLogicComponent(ComponentId componentId, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>> buttonTransformCompMapPtr);
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

    int selectedButtonIdx;
    std::vector<EMenuButtons> buttons;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<TransformComponent>>> m_buttonTransformCompPtrMapPtr;

    uint32_t lastTickVal;
};

