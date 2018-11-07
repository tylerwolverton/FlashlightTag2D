#pragma once
#include <cstdint>

#include "LogicComponent.h"

class GraphicsComponent;

class MainMenuLogicComponent :
    public LogicComponent
{
public:
    MainMenuLogicComponent(ComponentId componentId, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<GraphicsComponent>>> buttonGraphicsCompMapPtr);
    virtual ~MainMenuLogicComponent();

    void MoveUp() override;
    void MoveDown() override;
    void Select() override;

private:
    enum EMenuButtons
    {
        Play,
        Controls,
        Exit
    };

    int selectedButtonIdx;
    std::vector<EMenuButtons> buttons;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<GraphicsComponent>>> m_buttonGraphicsCompMapPtr;

    uint32_t lastTickVal;
};

