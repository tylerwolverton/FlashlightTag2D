#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "BaseLogicComponent.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "AIComponent.h"
#include "FollowTargetAIComponent.h"
#include "GameStateComponent.h"
#include "Command.h"

GameActor::GameActor(ActorId actorId, ComponentList components)
	: m_actorId(actorId),
      m_components(components)
{
	m_pCommands = std::make_shared<CommandList>();
}

GameActor::~GameActor()
{
}

void GameActor::Update(float deltaMs, uint32_t input)
{
	m_input = input;

	for (auto comp : m_components)
	{
		comp->Update(*this, deltaMs);

		for (auto command : *m_pCommands)
		{
			if (command)
			{
				command->Execute(*this);
			}
		}
        m_pCommands->clear();
	}
}

StrongActorComponentPtr GameActor::getComponentByName(EComponentNames componentName) const
{
	for (auto component : m_components)
	{
		if (component->GetComponentName() == componentName)
		{
			return component;
		}
	}

	return nullptr;
}

StrongAIComponentPtr GameActor::GetAIComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::AIComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<AIComponent>(component);
}

StrongBaseLogicComponentPtr GameActor::GetBaseLogicComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::BaseLogicComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<BaseLogicComponent>(component);
}

StrongFollowTargetAIComponentPtr GameActor::GetFollowTargetAIComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::FollowTargetAIComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<FollowTargetAIComponent>(component);
}

StrongGraphicsComponentPtr GameActor::GetGraphicsComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::GraphicsComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<GraphicsComponent>(component);
}

StrongInputComponentPtr GameActor::GetInputComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::InputComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<InputComponent>(component);
}

StrongPhysicsComponentPtr GameActor::GetPhysicsComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::PhysicsComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<PhysicsComponent>(component);
}

StrongTransformComponentPtr GameActor::GetTransformComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::TransformComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<TransformComponent>(component);
}

StrongGameStateComponentPtr GameActor::GetGameStateComponent() const
{
	StrongActorComponentPtr component = getComponentByName(EComponentNames::GameStateComponentEnum);
	if (component == nullptr)
	{
		return nullptr;
	}

	return std::dynamic_pointer_cast<GameStateComponent>(component);
}