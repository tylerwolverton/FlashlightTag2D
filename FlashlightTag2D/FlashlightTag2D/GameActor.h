#pragma once
#include <memory>
#include <cstdint>
#include "Types.h"
#include "Vector2D.h"

//struct SDL_Texture;
class AIComponent;
class BaseLogicComponent;
class FollowTargetAIComponent;
class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class AIComponent;
class TransformComponent;

class GameActor 
{
public:
    GameActor(ComponentList components, std::string actorClassName);
    virtual ~GameActor();

    virtual void Update(int delatMs, uint32_t input = 0);

    // TODO: Move to a new base Actor class in engine
    //template <class T>
    //std::shared_ptr<T> GameActor::GetComponent(EComponentNames componentName)
    //{
    //    StrongActorComponentPtr foundComponent;
    //    for (auto component : m_components)
    //    {
    //        if (component->GetComponentName() == componentName)
    //        {
    //            foundComponent = component;
    //            break;
    //        }
    //    }

    //    if (foundComponent == nullptr)
    //    {
    //        return nullptr;
    //    }

    //    return std::dynamic_pointer_cast<T>(foundComponent);
    //}

    
    StrongAIComponentPtr             GetAIComponent();
    StrongBaseLogicComponentPtr	     GetBaseLogicComponent();
    StrongFollowTargetAIComponentPtr GetFollowTargetAIComponent();
    StrongGraphicsComponentPtr	     GetGraphicsComponent();
    StrongInputComponentPtr	         GetInputComponent();
    StrongPhysicsComponentPtr	     GetPhysicsComponent();
    StrongTransformComponentPtr	     GetTransformComponent();

    uint32_t GetInput() { return m_input; }
    void SetInput(uint32_t input) { m_input = input; }

    std::shared_ptr<CommandList> GetCommands() { return m_pCommands; }
    void SetCommands(std::shared_ptr<CommandList> commands) { m_pCommands = commands; }

    std::string GetActorClassName() { return m_actorClassName; }

private:
    StrongActorComponentPtr getComponentByName(EComponentNames componentName);

    ComponentList m_components;
    uint32_t m_input;
    std::shared_ptr<CommandList> m_pCommands;
    std::string m_actorClassName;
};

