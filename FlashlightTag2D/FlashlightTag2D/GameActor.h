#pragma once
#include "Types.h"

class AIComponent;
class BaseLogicComponent;
class FollowTargetAIComponent;
class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class AIComponent;
class TransformComponent;
class GameStateComponent;

class GameActor 
{
public:
    GameActor(ComponentList components);
    virtual ~GameActor();

    virtual void Update(float delatMs, uint32_t input = 0);
        
    StrongAIComponentPtr             GetAIComponent();
    StrongBaseLogicComponentPtr	     GetBaseLogicComponent();
    StrongFollowTargetAIComponentPtr GetFollowTargetAIComponent();
    StrongGraphicsComponentPtr	     GetGraphicsComponent();
    StrongInputComponentPtr	         GetInputComponent();
    StrongPhysicsComponentPtr	     GetPhysicsComponent();
    StrongTransformComponentPtr	     GetTransformComponent();
	StrongGameStateComponentPtr	     GetGameStateComponent();

    uint32_t GetInput() { return m_input; }
    void SetInput(uint32_t input) { m_input = input; }

    std::shared_ptr<CommandList> GetCommands() { return m_pCommands; }
    void SetCommands(std::shared_ptr<CommandList> commands) { m_pCommands = commands; }

private:
    StrongActorComponentPtr getComponentByName(EComponentNames componentName);

    ComponentList m_components;
    uint32_t m_input;
    std::shared_ptr<CommandList> m_pCommands;
};

