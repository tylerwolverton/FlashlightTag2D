#pragma once
#include "ActorComponent.h"
#include "GameTypes.h"
#include "Vector2D.h"

#include <string>
#include <memory>

class Behavior;

// This is a class that would be good for actor type, health, status effects, game role, etc.
class GameStateComponent :
	public ActorComponent
{
public:
    GameStateComponent(ComponentId componentId, std::string actorName, EGameBehavior role);// , const Vector2D<int>& levelSize);
	virtual ~GameStateComponent();

	virtual void Update(GameActor& actor, float deltaMs) override;

	virtual const EComponentNames GetComponentName() const override;

	EGameBehavior GetRole() { return m_curRole; }
	void SetRole(EGameBehavior role) { m_curRole = role; updateBehavior(); }

    const std::string GetName() const { return m_actorName; }

	std::shared_ptr<Behavior> GetBehavior() { return m_behavior; }

private:
	void updateBehavior();

	EGameBehavior m_curRole;
	EGameBehavior m_prevRole;
	std::string m_actorName;
	std::shared_ptr<Behavior> m_behavior;
	//Vector2D<int> m_levelSize;
};