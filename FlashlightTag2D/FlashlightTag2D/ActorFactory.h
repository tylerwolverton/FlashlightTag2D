#pragma once
#include "document.h"

#include "Types.h"
#include "Vector2D.h"
#include <vector>
#include <memory>

class GameActor;
class GameStateComponent;
class PhysicsManager;
class GraphicsManager;
class Level;

class ActorFactory
{
public:
	ActorFactory();
	
    void CreateActorsFromJSONArray(const rapidjson::Value& actorList, PhysicsManager& physicsMgr, GraphicsManager& graphicsMgr, std::shared_ptr<Level> level);
	std::vector<std::shared_ptr<GameActor>> GetActorList() { return m_pEntityVec; }
	void ChooseSeekers(int seekerCount);
	void ClearActors();

    // Move to graphics manager
    std::shared_ptr<GameActor> CreateCamera(const Vector2D<int>& levelSize);
    std::shared_ptr<GameActor> CreateCamera(const std::shared_ptr<GameActor>& target, const Vector2D<int>& levelSize);

private:
	ActorId m_lastActorId;
	ActorId getNextActorId() { ++m_lastActorId; return m_lastActorId; };
    ComponentId m_lastComponentId;
    ComponentId getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };
	std::vector<std::shared_ptr<GameActor>> m_pEntityVec;
	
    // TODO: Cache changes
    //std::vector<GameActor> m_entityVec;

	std::vector<std::shared_ptr<GameStateComponent>> m_gameStateComponentVec;

    // Only needed for camera
    std::shared_ptr<GameActor> m_pCurrentPlayer;
};

