#include "GameTile.h"
#include "GameActor.h"
#include "PhysicsComponent.h"

GameTile::GameTile(int spriteIdx, std::shared_ptr<TransformComponent> transformCompPtr, std::shared_ptr<PhysicsComponent> physicsCompPtr)
    : m_spriteIdx(spriteIdx),
      m_transformCompPtr(transformCompPtr),
      m_physicsCompPtr(physicsCompPtr)
{
}

GameTile::~GameTile()
{
	m_idToActorPtrMap.clear();
    //m_gameActorVec.clear();
}

void GameTile::AddActor(std::shared_ptr<GameActor> gameActor)
{
    ActorId actorId = gameActor->GetActorId();
    if (m_idToActorPtrMap.find(actorId) == m_idToActorPtrMap.end())
    {
		m_idToActorPtrMap.insert(std::make_pair(actorId, gameActor));
    }

    //m_gameActorVec.push_back(gameActor);
}

void GameTile::RemoveActor(ActorId actorId)
{
    if (m_idToActorPtrMap.find(actorId) != m_idToActorPtrMap.end())
    {
		m_idToActorPtrMap.erase(actorId);
    }

    /*int idx = 0;
    for (auto actor : m_gameActorVec)
    {
        if (actor->GetActorId() == actorId)
        {
            break;
        }
        idx++;
    }

    m_gameActorVec.erase(m_gameActorVec.begin() + idx);*/
}

void GameTile::RemoveAllActors()
{
	m_idToActorPtrMap.clear();
}

std::shared_ptr<GameActor> GameTile::GetActorOnTile(ActorId actorId)
{
    auto actorMapIter = m_idToActorPtrMap.find(actorId);
    if (actorMapIter != m_idToActorPtrMap.end())
    {
        return actorMapIter->second;
    }

    return nullptr;
}