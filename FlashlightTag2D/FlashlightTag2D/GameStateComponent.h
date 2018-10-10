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
    GameStateComponent(ComponentId componentId, std::string actorName);
	virtual ~GameStateComponent();

	virtual void Update(GameActor& actor, float deltaMs) override;

	virtual const EComponentNames GetComponentName() const override;
    
    const std::string GetName() const { return m_actorName; }

private:
	std::string m_actorName;
};