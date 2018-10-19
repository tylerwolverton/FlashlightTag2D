#pragma once
#include "Types.h"

#include <vector>
#include <memory>

class Command;
class ActorComponent;
class AIComponent;
class LogicComponent;
class CameraFollowComponent;
class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class AIComponent;
class TransformComponent;
class GameStateComponent;
class LifeComponent;

class GameActor 
{
	friend class ActorFactory;

public:
    GameActor(ActorId actorId, std::string actorName, ComponentMap components);
    GameActor(ActorId actorId, std::string actorName);
    virtual ~GameActor();

    virtual void Update(float delatMs, InputData input);
    
	void InsertComponent(EComponentNames compName, std::shared_ptr<ActorComponent> comp);

    std::shared_ptr<AIComponent>             GetAIComponent() const;
    std::shared_ptr<LogicComponent>	         GetLogicComponent() const;
    std::shared_ptr<CameraFollowComponent>   GetCameraFollowComponent() const;
    std::shared_ptr<GraphicsComponent>	     GetGraphicsComponent() const;
    std::shared_ptr<InputComponent>	         GetInputComponent() const;
    std::shared_ptr<PhysicsComponent>	     GetPhysicsComponent() const;
    std::shared_ptr<TransformComponent>	     GetTransformComponent() const;
	std::shared_ptr<GameStateComponent>	     GetGameStateComponent() const;
    std::shared_ptr<LifeComponent>	         GetLifeComponent() const;

    void RemoveAIComponent();
    void RemoveLogicComponent();
    void RemoveCameraFollowComponent();
    void RemoveGraphicsComponent();
    void RemoveInputComponent();
    void RemovePhysicsComponent();
    void RemoveTransformComponent();
    void RemoveGameStateComponent();
    void RemoveLifeComponent();

    const InputData GetInput() const { return m_input; }

    void SetCommands(std::shared_ptr<std::vector<std::shared_ptr<Command>>> commands) { m_pCommands = commands; }

    const ActorId GetActorId() const { return m_actorId; }
	const std::string GetActorName() const { return m_actorName; }

	// Component communication methods
	Vector2D<float> GetMousePosition();

private:
    std::shared_ptr<ActorComponent> getComponentByName(EComponentNames compName) const;
    void updateComponent(EComponentNames compName, float deltaMs);

    ComponentMap m_componentMap;
    InputData m_input;
    std::shared_ptr<std::vector<std::shared_ptr<Command>>> m_pCommands;
    ActorId m_actorId;
	std::string m_actorName;
    // TODO: Cache changes
	//std::vector<int> m_componentIndexVec;
};

