#include "SeekBehavior.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "Vector2D.h"
#include "Command.h"

SeekBehavior::SeekBehavior()
{
    m_targetActor = nullptr;
    m_currState = EState::Search;
    m_maxChaseDistance = 150.0f;
    m_minTagDistance = 50.0f;
}

SeekBehavior::~SeekBehavior()
{
}

CommandList SeekBehavior::Update(GameActor& thisActor)
{
    // Use the current light cone and sound in surrounding area to see if any actors are close, then if a hider is seen, enter the chase state
    // If already chasing and the hider is lost for long enough, enter the search state (create a ChaseBahavior?)

    auto thisActorTransformComponent = thisActor.GetTransformComponent();
    if (thisActorTransformComponent == nullptr)
    {
        return CommandList();
    }

    if(m_currState == EState::Search)
    {
        // Get list of nearby actors
        auto actorList = ServiceLocator::GetActorFactory()->GetActorList();

        // Find closest hider in that list
        // Implicitly set max range, variable-ize this later
        auto distToClosestActor = 300.0f;
        for (auto otherActor : actorList)
        {
            // TODO: Rethink this type naming system to find the right game actors
            // Create game state component? Contains actor type, enum for seeker or hider, etc.
            auto actorName = otherActor->GetActorClassName();
            if (//*otherActor != thisActor && 
                (actorName == "Enemy" || actorName == "Player"))
            {
                auto otherActorTransformComponent = otherActor->GetTransformComponent();
                if (otherActorTransformComponent == nullptr)
                {
                    continue;
                }
                
                auto distToOtherActor = (otherActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition()).Length();
                if (distToOtherActor > 5.0f
                    && distToOtherActor < distToClosestActor)
                {
                    m_targetActor = otherActor;
                    distToClosestActor = distToOtherActor;
                }
            }
        }
        
        if(m_targetActor != nullptr)
        {
            m_currState = EState::Chase;
        } 
        else
        {
            return moveInSearchPattern();
        }
    }
    else if(m_currState == EState::Chase)
    {
        auto targetActorTransformComponent = m_targetActor->GetTransformComponent();
        if (targetActorTransformComponent == nullptr)
        {
            return CommandList();
        }

        auto distToTargetActor = (targetActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition()).Length();
        if (distToTargetActor < m_minTagDistance)
        {
            tagTarget();
        } 
        else if (distToTargetActor < m_maxChaseDistance)
        {
            return moveTowardsTarget(thisActorTransformComponent, targetActorTransformComponent);
        }
        
        // If we tagged the target or they are too far away, return to search state
        m_targetActor = nullptr;
        m_currState = EState::Search;
    }

    // Weird state, no actions taken
    return CommandList();
}

CommandList SeekBehavior::moveTowardsTarget(StrongTransformComponentPtr thisActorTransformComponent, 
                                            StrongTransformComponentPtr targetActorTransformComponent)
{
    CommandList commandList;

    auto distBetweenActors = targetActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition();

    if (distBetweenActors.x < -1.0f)
    {
        commandList.push_back(std::make_shared<MoveLeft>());
    }
    else if (distBetweenActors.x > 1.0f)
    {
        commandList.push_back(std::make_shared<MoveRight>());
    }

    if (distBetweenActors.y < -1.0f)
    {
        commandList.push_back(std::make_shared<MoveDown>());
    }
    else if (distBetweenActors.y > 1.0f)
    {
        commandList.push_back(std::make_shared<MoveUp>());
    }

    return commandList;
}

CommandList SeekBehavior::moveInSearchPattern()
{


    return CommandList();
}

CommandList SeekBehavior::tagTarget()
{


    return CommandList();
}