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
    GameActor(ActorId actorId, ComponentList components);
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
    //void SetInput(uint32_t input) { m_input = input; }

    CommandListPtr GetCommands() { return m_pCommands; }
    void SetCommands(CommandListPtr commands) { m_pCommands = commands; }

    const ActorId GetActorId() const { return m_actorId; }

private:
    StrongActorComponentPtr getComponentByName(EComponentNames componentName);

    ComponentList m_components;
    uint32_t m_input;
    CommandListPtr m_pCommands;
    ActorId m_actorId;
};

