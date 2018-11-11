#pragma once
#include "GameStateComponent.h"

#include <map>
#include <string>

class PlayerGameStateComponent :
    public GameStateComponent
{
public:
    PlayerGameStateComponent(ComponentId componentId, std::string actorName);
    virtual ~PlayerGameStateComponent();

    void AddToInventory(std::shared_ptr<GameStateComponent> gameStateComp);
    void RemoveFromInventoryByName(std::string compName);
    bool InventoryContainsItem(std::string name);
    std::shared_ptr<GameStateComponent> GetInventoryItemByName(std::string name);

private:    
    std::map<std::string, std::shared_ptr<GameStateComponent>> m_inventory;
};

