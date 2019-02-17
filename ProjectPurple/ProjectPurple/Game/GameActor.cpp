#include "GameActor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "LogicComponent.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "AIComponent.h"
#include "CameraFollowComponent.h"
#include "GameStateComponent.h"
#include "LifeComponent.h"
#include "Command.h"

GameActor::GameActor(ActorId actorId, const std::string& actorName, ComponentMap components)
    : m_actorId(actorId),
      m_actorName(actorName),
      m_componentMap(components)
{
    m_pCommands = std::make_shared<std::vector<std::shared_ptr<Command>>>();
}

// TODO: Cache changes
GameActor::GameActor(ActorId actorId, const std::string& actorName)
    : m_actorId(actorId),
      m_actorName(actorName)
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

void GameActor::Update(float deltaMs, InputData input)
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
    updateComponent(EComponentNames::LogicComponentEnum, deltaMs);
    //updateComponent(EComponentNames::GraphicsComponentEnum, deltaMs);
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

std::shared_ptr<ActorComponent> GameActor::getComponentByName(EComponentNames compName) const
{
    auto compIter = m_componentMap.find(compName);
    if (compIter != m_componentMap.end())
    {
        return compIter->second;
    }

    return nullptr;
}

void GameActor::InsertCompPtr(EComponentNames compName, const std::shared_ptr<ActorComponent>& comp)
{
    if (m_componentMap.find(compName) != m_componentMap.end())
    {
        m_componentMap[compName] = comp;
    }
    else
    {
        auto compPair = std::make_pair(compName, comp);
        m_componentMap.insert(compPair);
    }

    comp->SetParentActorId(m_actorId);
}

std::shared_ptr<AIComponent> GameActor::GetAICompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::AIComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<AIComponent>(component);
}

std::shared_ptr<LogicComponent> GameActor::GetLogicCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::LogicComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<LogicComponent>(component);
}

std::shared_ptr<CameraFollowComponent> GameActor::GetCameraFollowCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::CameraFollowComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<CameraFollowComponent>(component);
}

//std::shared_ptr<GraphicsComponent> GameActor::GetGraphicsCompPtr() const
//{
//    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::GraphicsComponentEnum);
//    if (component == nullptr)
//    {
//        return nullptr;
//    }
//
//    return std::dynamic_pointer_cast<GraphicsComponent>(component);
//}

std::shared_ptr<InputComponent> GameActor::GetInputCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::InputComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<InputComponent>(component);
}

std::shared_ptr<PhysicsComponent> GameActor::GetPhysicsCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::PhysicsComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<PhysicsComponent>(component);
}

std::shared_ptr<TransformComponent> GameActor::GetTransformCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::TransformComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<TransformComponent>(component);
}

std::shared_ptr<GameStateComponent> GameActor::GetGameStateCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::GameStateComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<GameStateComponent>(component);
}

std::shared_ptr<LifeComponent> GameActor::GetLifeCompPtr() const
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::LifeComponentEnum);
    if (component == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<LifeComponent>(component);
}

void GameActor::RemoveAICompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::AIComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::AIComponentEnum);
    }
}

void GameActor::RemoveLogicCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::LogicComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::LogicComponentEnum);
    }
}

void GameActor::RemoveCameraFollowCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::CameraFollowComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::CameraFollowComponentEnum);
    }
}

void GameActor::RemoveGraphicsCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::GraphicsComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::GraphicsComponentEnum);
    }
}

void GameActor::RemoveInputCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::InputComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::InputComponentEnum);
    }
}

void GameActor::RemovePhysicsCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::PhysicsComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::PhysicsComponentEnum);
    }
}

void GameActor::RemoveTransformCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::TransformComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::TransformComponentEnum);
    }
}

void GameActor::RemoveGameStateCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::GameStateComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::GameStateComponentEnum);
    }
}

void GameActor::RemoveLifeCompPtr()
{
    std::shared_ptr<ActorComponent> component = getComponentByName(EComponentNames::LifeComponentEnum);
    if (component != nullptr)
    {
        m_componentMap.erase(EComponentNames::LifeComponentEnum);
    }
}

Vector2D<float> GameActor::GetMousePosition()
{
    Vector2D<float> pos(0.0f, 0.0f);

    auto inputCompPtr = GetInputCompPtr();
    if (inputCompPtr != nullptr)
    {
        pos = Vector2D<float>((float)inputCompPtr->GetOldMousePos().x, (float)inputCompPtr->GetOldMousePos().y);
    }

    return pos;
}