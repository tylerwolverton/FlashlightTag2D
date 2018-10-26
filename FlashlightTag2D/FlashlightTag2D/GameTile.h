#pragma once
#include "Types.h"

#include <map>
#include <memory>

class TransformComponent;
class PhysicsComponent;
class GameActor;

class GameTile
{
public:
    GameTile(int spriteIdx, std::shared_ptr<TransformComponent> transformComp, std::shared_ptr<PhysicsComponent> physicsComp = nullptr);
    virtual ~GameTile();
        
    int GetSpriteIdx() { return m_spriteIdx; }
    std::shared_ptr<TransformComponent> GetTransformComponent() { return m_transformComp; };
    std::shared_ptr<PhysicsComponent> GetPhysicsComponent() { return m_physicsComp; };

    void AddActor(std::shared_ptr<GameActor> gameActor);
    void RemoveActor(ActorId actorId);
    std::shared_ptr<GameActor> GetActorOnTile(ActorId actorId);

private:
    int m_spriteIdx;
    std::shared_ptr<TransformComponent> m_transformComp;
    std::shared_ptr<PhysicsComponent> m_physicsComp;
    std::map<ActorId, std::shared_ptr<GameActor>> m_gameActorMap;
};

