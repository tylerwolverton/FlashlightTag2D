#pragma once
#include "ActorComponent.h"
#include "GameTypes.h"
#include "Vector2D.h"

#include <string>
#include <memory>

class Behavior;

class GameStateComponent :
    public ActorComponent
{
public:
    GameStateComponent(ComponentId componentId, const std::string& actorName, const std::string& actorType);
    virtual ~GameStateComponent();

    virtual void Update(GameActor& actor, float deltaMs) override;

    virtual const EComponentNames GetComponentName() const override;
    
    const std::string GetName() const { return m_actorName; }
    const std::string GetType() const { return m_actorType; }

private:
    std::string m_actorName;
    std::string m_actorType;
};