#include "GameTile.h"
#include "GameActor.h"
#include "PhysicsComponent.h"

GameTile::GameTile(int spriteIdx, std::shared_ptr<PhysicsComponent> physicsComp)
    : m_spriteIdx(spriteIdx),
	  m_physicsComp(physicsComp)
{
}

GameTile::~GameTile()
{
    m_gameActorMap.clear();
}

void GameTile::AddActor(std::shared_ptr<GameActor> gameActor)
{
    ActorId actorId = gameActor->GetActorId();
    if (m_gameActorMap.find(actorId) == m_gameActorMap.end())
    {
        m_gameActorMap.insert(std::make_pair(actorId, gameActor));
    }
}

void GameTile::RemoveActor(ActorId actorId)
{
    if (m_gameActorMap.find(actorId) != m_gameActorMap.end())
    {
        m_gameActorMap.erase(actorId);
    }
}

std::shared_ptr<GameActor> GameTile::GetActorOnTile(ActorId actorId)
{
    auto actorIter = m_gameActorMap.find(actorId);
    if (actorIter != m_gameActorMap.end())
    {
        return actorIter->second;
    }

    return nullptr;
}