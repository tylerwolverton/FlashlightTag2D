#pragma once
#include "LogicComponent.h"
#include <string>

class PortalLogicComponent :
    public LogicComponent
{
public:
    PortalLogicComponent(ComponentId componentId, std::string levelName);
    virtual ~PortalLogicComponent();

    void ChangeLevel();

private:
    std::string m_levelName;
};

