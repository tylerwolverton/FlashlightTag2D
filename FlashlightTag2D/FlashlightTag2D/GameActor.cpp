#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "BaseLogicComponent.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "AIComponent.h"
#include "CameraFollowComponent.h"
#include "GameStateComponent.h"
#include "Command.h"

GameActor::GameActor(ActorId actorId, ComponentMap components)
    : m_actorId(actorId),
    m_componentMap(components)
{
    m_pCommands = std::make_shared<std::vector<std::shared_ptr<Command>>>();
}

// TODO: Cache changes
GameActor::GameActor(ActorId actorId)
	: m_actorId(actorId)
{
	m_pCommands = std::make_shared<std::vector<std::shared_ptr<Command>>>();

    // TODO: Cache changes
	//for (int i = 0; i < ComponentTypeCount; i++)
	//{
	//	m_componentIndexVec.push_back(-1);
	//}
}

GameActor::~GameActor()
{
	m_componentMap.clear();
	m_pCommands.reset();
}

void GameActor::Update(float deltaMs, uint32_t input)
{
    m_input = input;
    updateComponent(EComponentNames::InputComponentEnum, deltaMs);
    updateComponent(EComponentNames::AIComponentEnum, deltaMs);
    updateComponent(EComponentNames::CameraFollowComponentEnum, deltaMs);

    for (auto command : *m_pCommands)
    {
        if (command)
        {
            command->Execute(*this);
        }
    }
    m_pCommands->clear();
    
    updateComponent(EComponentNames::PhysicsComponentEnum, deltaMs);
    updateComponent(EComponentNames::BaseLogicComponentEnum, deltaMs);
    updateComponent(EComponentNames::GraphicsComponentEnum, deltaMs);
    updateComponent(EComponentNames::GameStateComponentEnum, deltaMs);
}

void GameActor::updateComponent(EComponentNames compName, float deltaMs)
{
    auto compIter = m_componentMap.find(compName);
    if (compIter != m_componentMap.end())
    {
        compIter->second->Update(*this, deltaMs);
    }
}

std::shared_ptr<ActorComponent> GameActor::getComponentByName(EComponentNames componentName) const
{
    auto compIter = m_componentMap.find(componentName);
    if (compIter != m_componentMap.end())
    {
        return compIter->second;
    }

    return nullptr;
}

std::shared_ptr<AIComponent> GameActor::GetAIComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::AIComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<AIComponent>(component);
}

std::shared_ptr<BaseLogicComponent> GameActor::GetBaseLogicComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::BaseLogicComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<BaseLogicComponent>(component);
}

std::shared_ptr<CameraFollowComponent> GameActor::GetCameraFollowComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::CameraFollowComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<CameraFollowComponent>(component);
}

std::shared_ptr<GraphicsComponent> GameActor::GetGraphicsComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::GraphicsComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<GraphicsComponent>(component);
}

std::shared_ptr<InputComponent> GameActor::GetInputComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::InputComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<InputComponent>(component);
}

std::shared_ptr<PhysicsComponent> GameActor::GetPhysicsComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::PhysicsComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<PhysicsComponent>(component);
}

std::shared_ptr<TransformComponent> GameActor::GetTransformComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::TransformComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<TransformComponent>(component);
}

std::shared_ptr<GameStateComponent> GameActor::GetGameStateComponent() const
{
	std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::GameStateComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<GameStateComponent>(component);
}