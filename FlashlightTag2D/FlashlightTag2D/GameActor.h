#pragma once
#include "Types.h"

#include <vector>

class AIComponent;
class BaseLogicComponent;
class CameraFollowComponent;
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
    GameActor(ActorId actorId, ComponentMap components);
    GameActor(ActorId actorId);
    virtual ~GameActor();

    virtual void Update(float delatMs, uint32_t input = 0);
        
    StrongAIComponentPtr             GetAIComponent() const;
    StrongBaseLogicComponentPtr	     GetBaseLogicComponent() const;
    CameraFollowComponentPtr         GetCameraFollowComponent() const;
    StrongGraphicsComponentPtr	     GetGraphicsComponent() const;
    StrongInputComponentPtr	         GetInputComponent() const;
    StrongPhysicsComponentPtr	     GetPhysicsComponent() const;
    StrongTransformComponentPtr	     GetTransformComponent() const;
	StrongGameStateComponentPtr	     GetGameStateComponent() const;

    const uint32_t GetInput() const { return m_input; }

    void SetCommands(CommandListPtr commands) { m_pCommands = commands; }

    const ActorId GetActorId() const { return m_actorId; }

private:
    StrongActorComponentPtr getComponentByName(EComponentNames componentName) const;
    void updateComponent(EComponentNames compName, float deltaMs);

    ComponentMap m_componentMap;
    uint32_t m_input;
    CommandListPtr m_pCommands;
    ActorId m_actorId;
    // TODO: Cache changes
	//std::vector<int> m_componentIndexVec;
};

