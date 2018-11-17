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
    GameTile(int spriteIdx, 
             const std::shared_ptr<TransformComponent>& transformCompPtr, 
             const std::shared_ptr<PhysicsComponent>& physicsCompPtr = nullptr);
    virtual ~GameTile();
        
    int GetSpriteIdx() { return m_spriteIdx; }
    std::shared_ptr<TransformComponent> GetTransformCompPtr() const { return m_transformCompPtr; }
    std::shared_ptr<PhysicsComponent> GetPhysicsCompPtr() const { return m_physicsCompPtr; }
    std::map<ActorId, std::shared_ptr<GameActor>> GetActorMap() const { return m_idToActorPtrMap; }

    void AddActor(const std::shared_ptr<GameActor>& gameActor);
    void RemoveActor(ActorId actorId);
    void RemoveAllActors();
    std::shared_ptr<GameActor> GetActorOnTile(ActorId actorId) const;

private:
    int m_spriteIdx;
    std::shared_ptr<TransformComponent> m_transformCompPtr;
    std::shared_ptr<PhysicsComponent> m_physicsCompPtr;
    std::map<ActorId, std::shared_ptr<GameActor>> m_idToActorPtrMap;
};

