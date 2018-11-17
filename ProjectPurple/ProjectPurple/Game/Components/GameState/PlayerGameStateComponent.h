#pragma once
#include "GameStateComponent.h"

#include <map>
#include <string>

class PlayerGameStateComponent :
    public GameStateComponent
{
public:
    PlayerGameStateComponent(ComponentId componentId, const std::string& actorName);
    virtual ~PlayerGameStateComponent();

    void AddToInventory(const std::shared_ptr<GameStateComponent>& gameStateComp);
    void RemoveFromInventoryByName(const std::string& compName);
    bool InventoryContainsItem(const std::string& name) const;
    std::shared_ptr<GameStateComponent> GetInventoryItemByName(const std::string& name) const;

private:    
    std::map<std::string, std::shared_ptr<GameStateComponent>> m_inventory;
};

