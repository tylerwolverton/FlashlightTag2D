#pragma once
#include "Types.h"

#include <map>
#include <vector>
#include <memory>

class TransformComponent;
class PhysicsComponent;
class GameActor;

class GameTile
{
public:
    GameTile(int spriteIdx, std::shared_ptr<TransformComponent> transformCompPtr, std::shared_ptr<PhysicsComponent> physicsCompPtr = nullptr);
    virtual ~GameTile();
        
    int GetSpriteIdx() { return m_spriteIdx; }
    std::shared_ptr<TransformComponent> GetTransformCompPtr() { return m_transformCompPtr; }
    std::shared_ptr<PhysicsComponent> GetPhysicsCompPtr() { return m_physicsCompPtr; }
    std::map<ActorId, std::shared_ptr<GameActor>> GetActorMap() { return m_idToActorPtrMap; }

    void AddActor(std::shared_ptr<GameActor> gameActor);
    void RemoveActor(ActorId actorId);
    void RemoveAllActors();
    std::shared_ptr<GameActor> GetActorOnTile(ActorId actorId);

private:
    int m_spriteIdx;
    std::shared_ptr<TransformComponent> m_transformCompPtr;
    std::shared_ptr<PhysicsComponent> m_physicsCompPtr;
    std::map<ActorId, std::shared_ptr<GameActor>> m_idToActorPtrMap;
};

