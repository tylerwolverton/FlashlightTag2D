#pragma once
#include "document.h"

#include "Types.h"
#include "Vector2D.h"
#include <vector>
#include <map>
#include <memory>

class GameActor;
class GameStateComponent;
class PhysicsManager;
class GraphicsManager;
class Level;
class TransformComponent;
class PhysicsComponent;

class ActorFactory
{
public:
	ActorFactory(std::shared_ptr<PhysicsManager> physicsMgr, std::shared_ptr<GraphicsManager> graphicsMgr);
	
    //void CreateActorsFromJSONArray(const rapidjson::Value& actorList, std::shared_ptr<Level> level);
    //std::vector<std::shared_ptr<GameActor>> GetActorList() { return m_pEntityVec; }
    std::map<ActorId, std::shared_ptr<GameActor>> GetActorMap() { return m_pEntityMap; }
    std::shared_ptr<GameActor> GetActor(ActorId actorId);
    std::shared_ptr<GameActor> GetPlayer();
	std::shared_ptr<GameActor> GetCurrentCamera() { return m_pCurrentCamera; }

    void InitLevelActors(const rapidjson::Value& actorList, std::shared_ptr<Level> newLevel);
    //void ChooseSeekers(int seekerCount);
    void AddDeadActor(ActorId id) { m_deadActorVec.push_back(id); }
    void RemoveDeadActors();

	// Actor creation methods
    std::shared_ptr<GameActor> CreateEnemy(Vector2D<float> position);
	std::shared_ptr<GameActor> CreateProjectile(Vector2D<float> position, Vector2D<float> velocity);
	std::shared_ptr<GameActor> CreateActorFromName(std::string name, Vector2D<float> position, Vector2D<float> velocity = Vector2D<float>(0.0, 0.0));
	// Move to graphics manager
	std::shared_ptr<GameActor> CreateCamera(const Vector2D<int>& levelSize);
	std::shared_ptr<GameActor> CreateCamera(const std::shared_ptr<GameActor>& target, const Vector2D<int>& levelSize);

private:
	ActorId m_lastActorId;
	ActorId getNextActorId() { ++m_lastActorId; return m_lastActorId; };
    ComponentId m_lastComponentId;
    ComponentId getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };

	std::shared_ptr<GameActor> createActor(const char* const actorPath, Vector2D<float> position = Vector2D<float>(-1.0f, -1.0f));
	void addInputComponent(const char* const type, std::shared_ptr<GameActor> actor);
	std::shared_ptr<PhysicsComponent> addPhysicsComponent(const char* const type,
														  std::shared_ptr<GameActor> actor,
														  std::shared_ptr<TransformComponent> transformCompPtr,
														  float maxSpeed,
														  float mass,
														  float restitution);

	void addComponentsToManagers(std::shared_ptr<GameActor> actor);

    std::map<ActorId, std::shared_ptr<GameActor>> m_pEntityMap;
	std::vector<std::shared_ptr<GameActor>> m_pEntityVec;
    std::vector<ActorId> m_deadActorVec;

    std::shared_ptr<PhysicsManager> m_physicsMgr;
    std::shared_ptr<GraphicsManager> m_graphicsMgr;

    // TODO: Cache changes
    //std::vector<GameActor> m_entityVec;

	std::vector<std::shared_ptr<GameStateComponent>> m_gameStateComponentVec;

    // Only needed for camera
    std::shared_ptr<GameActor> m_pCurrentPlayer;
	std::shared_ptr<GameActor> m_pCurrentCamera;
};

