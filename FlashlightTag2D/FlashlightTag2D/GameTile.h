#pragma once
#include "Types.h"

#include <map>
#include <memory>

class PhysicsComponent;
class GameActor;

class GameTile
{
public:
    GameTile(int spriteIdx, std::shared_ptr<PhysicsComponent> physicsComp = nullptr);
    virtual ~GameTile();
        
    std::shared_ptr<PhysicsComponent> GetPhysicsComponent() { return m_physicsComp; };
    int GetSpriteIdx() { return m_spriteIdx; }

    void AddActor(std::shared_ptr<GameActor> gameActor);
    void RemoveActor(ActorId actorId);
    std::shared_ptr<GameActor> GetActorOnTile(ActorId actorId);

private:
    int m_spriteIdx;
    std::shared_ptr<PhysicsComponent> m_physicsComp;
    std::map<ActorId, std::shared_ptr<GameActor>> m_gameActorMap;
};

