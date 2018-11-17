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
class GameTile;

class GameActor 
{
    friend class ActorFactory;

public:
    GameActor(ActorId actorId, const std::string& actorName, ComponentMap components);
    GameActor(ActorId actorId, const std::string& actorName);
    virtual ~GameActor();

    virtual void Update(float delatMs, InputData input);
    
    void InsertCompPtr(EComponentNames compName, const std::shared_ptr<ActorComponent>& comp);

    std::shared_ptr<AIComponent>             GetAICompPtr() const;
    std::shared_ptr<LogicComponent>	         GetLogicCompPtr() const;
    std::shared_ptr<CameraFollowComponent>   GetCameraFollowCompPtr() const;
    std::shared_ptr<GraphicsComponent>	     GetGraphicsCompPtr() const;
    std::shared_ptr<InputComponent>	         GetInputCompPtr() const;
    std::shared_ptr<PhysicsComponent>	     GetPhysicsCompPtr() const;
    std::shared_ptr<TransformComponent>	     GetTransformCompPtr() const;
    std::shared_ptr<GameStateComponent>	     GetGameStateCompPtr() const;
    std::shared_ptr<LifeComponent>	         GetLifeCompPtr() const;

    void RemoveAICompPtr();
    void RemoveLogicCompPtr();
    void RemoveCameraFollowCompPtr();
    void RemoveGraphicsCompPtr();
    void RemoveInputCompPtr();
    void RemovePhysicsCompPtr();
    void RemoveTransformCompPtr();
    void RemoveGameStateCompPtr();
    void RemoveLifeCompPtr();

    const InputData GetInput() const { return m_input; }

    void SetCommands(const std::shared_ptr<std::vector<std::shared_ptr<Command>>>& commands) { m_pCommands = commands; }

    const ActorId GetActorId() const { return m_actorId; }
    const std::string GetActorName() const { return m_actorName; }

    const std::vector<std::shared_ptr<GameTile>> GetTileVec() const { return m_tileVec; }
    void SetTileVec(const std::vector<std::shared_ptr<GameTile>>& tileVec) { m_tileVec = tileVec; }

    // Component communication methods
    Vector2D<float> GetMousePosition();

private:
    std::shared_ptr<ActorComponent> getComponentByName(EComponentNames compName) const;
    void updateComponent(EComponentNames compName, float deltaMs);

    ComponentMap m_componentMap;
    InputData m_input;
    std::shared_ptr<std::vector<std::shared_ptr<Command>>> m_pCommands;
    std::vector<std::shared_ptr<GameTile>> m_tileVec;
    ActorId m_actorId;
    std::string m_actorName;
    // TODO: Cache changes
    //std::vector<int> m_componentIndexVec;
};

