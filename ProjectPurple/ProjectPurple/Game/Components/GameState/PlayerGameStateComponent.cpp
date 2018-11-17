#include "PlayerGameStateComponent.h"

PlayerGameStateComponent::PlayerGameStateComponent(ComponentId componentId, const std::string& actorName)
    : GameStateComponent(componentId, actorName, "Player"),
      m_inventory(std::map<std::string, std::shared_ptr<GameStateComponent>>())
{
}

PlayerGameStateComponent::~PlayerGameStateComponent()
{
}

void PlayerGameStateComponent::RemoveFromInventoryByName(const std::string& compName)
{
    if (m_inventory.find(compName) != m_inventory.end())
    {
        m_inventory.erase(compName);
    }
}

void PlayerGameStateComponent::AddToInventory(const std::shared_ptr<GameStateComponent>& gameStateComp)
{
    m_inventory.insert(std::make_pair(gameStateComp->GetName(), gameStateComp)); 
}

bool PlayerGameStateComponent::InventoryContainsItem(const std::string& name) const
{
    return GetInventoryItemByName(name) != nullptr;

}

std::shared_ptr<GameStateComponent> PlayerGameStateComponent::GetInventoryItemByName(const std::string& name) const
{
    auto inventoryIter = m_inventory.find(name);
    if (inventoryIter == m_inventory.end())
    {
        return nullptr;
    }
    
    return inventoryIter->second;
}