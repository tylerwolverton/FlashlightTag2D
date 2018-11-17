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
class LifeComponent;

class ActorFactory
{
public:
    ActorFactory(const std::shared_ptr<PhysicsManager>& physicsMgr, const std::shared_ptr<GraphicsManager>& graphicsMgr);
    
    std::map<ActorId, std::shared_ptr<GameActor>> GetActorMap() const { return m_idToActorPtrMap; }
    std::shared_ptr<GameActor> GetActor(ActorId actorId) const;
    std::shared_ptr<GameActor> GetFirstActorWithName(std::string actorName) const;
    std::shared_ptr<GameActor> GetPlayer() const;
    std::shared_ptr<GameActor> GetCurrentCamera() const { return m_curCameraPtr; }

    void InitLevelActors(const rapidjson::Value& actorList, const std::shared_ptr<Level>& newLevel);

    void KillAllActorsByName(const std::string& name);
    void AddDeadActor(ActorId id) { m_deadActorIdVec.push_back(id); }
    void RemoveDeadActors();

    // Actor creation methods
    std::shared_ptr<GameActor> CreateProjectile(const Vector2D<float>& position, 
                                                const Vector2D<float>& velocity);
    std::shared_ptr<GameActor> CreateActorFromName(const std::string& name, 
                                                   const Vector2D<float>& position, 
                                                   const Vector2D<float>& velocity = Vector2D<float>(0.0, 0.0));
    // TODO: Move to graphics manager
    std::shared_ptr<GameActor> CreateCamera(const Vector2D<int>& levelSize);
    std::shared_ptr<GameActor> CreateCamera(const std::shared_ptr<GameActor>& target, 
                                            const Vector2D<int>& levelSize);

private:
    ActorId m_lastActorId;
    ActorId getNextActorId() { ++m_lastActorId; return m_lastActorId; };
    ComponentId m_lastComponentId;
    ComponentId getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };

	std::map<ActorId, std::shared_ptr<GameActor>> m_idToActorPtrMap;
	std::vector<std::shared_ptr<GameActor>> m_actorPtrVec;
	std::vector<ActorId> m_deadActorIdVec;

	std::shared_ptr<PhysicsManager> m_physicsMgrPtr;
	std::shared_ptr<GraphicsManager> m_graphicsMgrPtr;

	// TODO: Cache changes
	//std::vector<GameActor> m_entityVec;

	std::vector<std::shared_ptr<GameStateComponent>> m_gameStateCompPtrVec;

	// Only needed for camera
	std::shared_ptr<GameActor> m_curPlayerPtr;
	std::shared_ptr<GameActor> m_curCameraPtr;

	// Persistence variables
	std::shared_ptr<GameStateComponent> m_persistedPlayerGameStateCompPtr;
	std::shared_ptr<LifeComponent> m_persistedPlayerLifeCompPtr;

    std::shared_ptr<GameActor> createActor(const char* const actorPath, 
                                           const Vector2D<float>& position = Vector2D<float>(-1.0f, -1.0f));

    void addInputComponent(const char* const type, 
                           const std::shared_ptr<GameActor>& actor);

    std::shared_ptr<PhysicsComponent> addPhysicsComponent(const char* const type,
                                                          const std::shared_ptr<GameActor>& actor,
                                                          const std::shared_ptr<TransformComponent>& transformCompPtr,
                                                          float maxSpeed,
                                                          float mass,
                                                          float restitution);

    void addComponentsToManagers(const std::shared_ptr<GameActor>& actorPtr);
};

