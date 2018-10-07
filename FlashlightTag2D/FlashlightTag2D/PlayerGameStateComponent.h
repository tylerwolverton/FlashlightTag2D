#pragma once
#include "GameStateComponent.h"

#include <map>

class GameActor;

class PlayerGameStateComponent :
    public GameStateComponent
{
public:
    PlayerGameStateComponent(ComponentId componentId, std::string actorName);
    virtual ~PlayerGameStateComponent();

    bool HasKey() { return m_hasKey; }
    void CollectKey() { m_hasKey = true; }
    void LoseKey() { m_hasKey = false; }

private:    
    // TODO: Create inventory
	std::map<ActorId, std::shared_ptr<GameActor>> m_inventory;
    bool m_hasKey;
};

