#pragma once
#include "Types.h"

#include <vector>

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
	friend class ActorFactory;

public:
    GameActor(ActorId actorId, ComponentList components);
	GameActor(ActorId actorId);
    virtual ~GameActor();

    virtual void Update(float delatMs, uint32_t input = 0);
        
    StrongAIComponentPtr             GetAIComponent() const;
    StrongBaseLogicComponentPtr	     GetBaseLogicComponent() const;
    StrongFollowTargetAIComponentPtr GetFollowTargetAIComponent() const;
    StrongGraphicsComponentPtr	     GetGraphicsComponent() const;
    StrongInputComponentPtr	         GetInputComponent() const;
    StrongPhysicsComponentPtr	     GetPhysicsComponent() const;
    StrongTransformComponentPtr	     GetTransformComponent() const;
	StrongGameStateComponentPtr	     GetGameStateComponent() const;

    const uint32_t GetInput() const { return m_input; }
    //void SetInput(uint32_t input) { m_input = input; }

    //CommandListPtr GetCommands() { return m_pCommands; }
    void SetCommands(CommandListPtr commands) { m_pCommands = commands; }

    const ActorId GetActorId() const { return m_actorId; }

private:
    StrongActorComponentPtr getComponentByName(EComponentNames componentName) const;

    ComponentList m_components;
    uint32_t m_input;
    CommandListPtr m_pCommands;
    ActorId m_actorId;
	std::vector<int> m_componentIndexVec;
};

