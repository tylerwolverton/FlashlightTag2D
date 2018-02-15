#pragma once
#include "ActorComponent.h"
#include "GameTypes.h"

// This is a class that would be good for actor type, health, status effects, game role, etc.
class GameStateComponent :
	public ActorComponent
{
public:
	GameStateComponent(std::string actorName, EGameRole role);
	virtual ~GameStateComponent();

	virtual void Update(GameActor& actor, float deltaMs) override;

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

	EGameRole GetRole() { return m_curRole; }
	void SetRole(EGameRole role) { m_curRole = role; }

    const std::string GetName() const { return m_actorName; }

	std::shared_ptr<Behavior> GetBehavior() { return m_behavior; }

private:
	void updateBehavior();

	EGameRole m_curRole;
	EGameRole m_prevRole;
	std::string m_actorName;
	std::shared_ptr<Behavior> m_behavior;
};