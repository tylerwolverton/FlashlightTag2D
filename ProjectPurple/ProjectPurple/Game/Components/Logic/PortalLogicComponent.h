#pragma once
#include "LogicComponent.h"
#include <string>

class PortalLogicComponent :
    public LogicComponent
{
public:
    PortalLogicComponent(ComponentId componentId, std::string levelName, float countdownTimeSec);
    virtual ~PortalLogicComponent();

    void ChangeLevel();
    void Update(GameActor& actor, float deltaMs) override;

private:
    std::string m_levelName;
    float m_countdownTimeSec;
    uint32_t m_firstTickVal;
};

