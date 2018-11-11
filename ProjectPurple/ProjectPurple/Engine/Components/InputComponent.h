#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

class Command;
class UpdateMousePosition;

class InputComponent : public ActorComponent
{
public:
    virtual ~InputComponent();
    virtual void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;

    Vector2D<int> GetOldMousePos() { return m_oldMousePos; }

protected:
    InputComponent(ComponentId componentId);

    std::shared_ptr<Command> m_wButtonCmdPtr;
    std::shared_ptr<Command> m_aButtonCmdPtr;
    std::shared_ptr<Command> m_sButtonCmdPtr;
    std::shared_ptr<Command> m_dButtonCmdPtr;
    std::shared_ptr<Command> m_upButtonCmdPtr;
    std::shared_ptr<Command> m_leftButtonPtr;
    std::shared_ptr<Command> m_downButtonCmdPtr;
    std::shared_ptr<Command> m_rightButtonCmdPtr;
    std::shared_ptr<Command> m_escButtonCmdPtr;
    std::shared_ptr<Command> m_spaceButtonCmdPtr;
    std::shared_ptr<Command> m_returnButtonCmdPtr;
    std::shared_ptr<Command> m_rightMouseButtonCmdPtr;
    std::shared_ptr<Command> m_leftMouseButtonCmdPtr;
    std::shared_ptr<Command> m_middleMouseButtonCmdPtr;
    std::shared_ptr<UpdateMousePosition> m_updateMousePositionPtr;

private:
    Vector2D<int> m_oldMousePos;
};

