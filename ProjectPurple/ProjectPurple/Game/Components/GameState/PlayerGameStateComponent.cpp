#include "PlayerGameStateComponent.h"

PlayerGameStateComponent::PlayerGameStateComponent(ComponentId componentId, std::string actorName)
    : GameStateComponent(componentId, actorName, "Player"),
      m_inventory(std::map<std::string, std::shared_ptr<GameStateComponent>>())
{
}

PlayerGameStateComponent::~PlayerGameStateComponent()
{
}

void PlayerGameStateComponent::RemoveFromInventoryByName(std::string compName)
{
    if (m_inventory.find(compName) != m_inventory.end())
    {
        m_inventory.erase(compName);
    }
}

void PlayerGameStateComponent::AddToInventory(std::shared_ptr<GameStateComponent> gameStateComp)
{
    m_inventory.insert(std::make_pair(gameStateComp->GetName(), gameStateComp)); 
}

bool PlayerGameStateComponent::InventoryContainsItem(std::string name)
{
    return GetInventoryItemByName(name) != nullptr;

}

std::shared_ptr<GameStateComponent> PlayerGameStateComponent::GetInventoryItemByName(std::string name)
{
    auto inventoryIter = m_inventory.find(name);
    if (inventoryIter == m_inventory.end())
    {
        return nullptr;
    }
    
    return inventoryIter->second;
}