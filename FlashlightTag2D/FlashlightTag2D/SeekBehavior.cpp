#include "SeekBehavior.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "Vector2D.h"
#include "Command.h"
#include "GameTypes.h"
#include "RandomNumberGenerator.h"

#include <random>
#include <algorithm>

SeekBehavior::SeekBehavior(const Vector2D<int>& levelSize)
  : m_levelSize(levelSize),
	m_targetActor(nullptr),
    m_currState(EState::Search),
    m_maxChaseDistance(151.0f),
    m_minTagDistance(100.0f),
	m_currentSearchPos(0),
	m_ticksSinceLastMove(0)
{
	initSearchPositions();
}

void SeekBehavior::initSearchPositions()
{
	m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(50, m_levelSize.x / 2), 
										        RandomNumberGenerator::GetIntWithinRange(50, m_levelSize.y / 2)));

	m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(m_levelSize.x / 2, m_levelSize.x - 50),
											    RandomNumberGenerator::GetIntWithinRange(50, m_levelSize.y / 2)));

	m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(50, m_levelSize.x / 2),
		                                        RandomNumberGenerator::GetIntWithinRange(m_levelSize.y / 2, m_levelSize.y - 50)));

	m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(m_levelSize.x / 2, m_levelSize.x - 50),
	                                            RandomNumberGenerator::GetIntWithinRange(m_levelSize.y / 2, m_levelSize.y - 50)));

	m_currentSearchPos = RandomNumberGenerator::GetIntWithinRange(0, m_searchPositions.size() - 1);
}

SeekBehavior::~SeekBehavior()
{
}

CommandList SeekBehavior::Update(const GameActor& thisActor)
{
    // Use the current light cone and sound in surrounding area to see if any actors are close, then if a hider is seen, enter the chase state
    // If already chasing and the hider is lost for long enough, enter the search state

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
        auto distToClosestActor = 150.0f;
        for (auto otherActor : actorList)
        {
			auto gameStateComponent = otherActor->GetGameStateComponent();
			if (gameStateComponent == nullptr)
			{
				continue;
			}
            
            if (//*otherActor != thisActor && 
				gameStateComponent->GetRole() == EGameRole::Hider)
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
            return moveInSearchPattern(thisActorTransformComponent);
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
			// Tag target
			// Set to either seeker or out depending on the game mode
			m_targetActor->GetGameStateComponent()->SetRole(EGameRole::Seeker);
        } 
        else if (distToTargetActor < m_maxChaseDistance)
        {
            return moveTowardsTarget(thisActorTransformComponent, targetActorTransformComponent);
        }
        
        // If we tagged the target or they are too far away, return to search state
        m_targetActor = nullptr;
        m_currState = EState::Search;
    }

    // No actions taken
    return CommandList();
}

CommandList SeekBehavior::moveTowardsTarget(StrongTransformComponentPtr thisActorTransformComponent, 
                                            StrongTransformComponentPtr targetActorTransformComponent)
{
	return moveToPosition(thisActorTransformComponent->GetPosition(), targetActorTransformComponent->GetPosition());
}

CommandList SeekBehavior::moveToPosition(Vector2D<float> currentPos,
					                     Vector2D<float> targetPos)
{
	CommandList commandList;

	auto dist = targetPos - currentPos;

	if (dist.x < -1.0f)
	{
		commandList.push_back(std::make_shared<MoveLeft>());
	}
	else if (dist.x > 1.0f)
	{
		commandList.push_back(std::make_shared<MoveRight>());
	}

	if (dist.y < -1.0f)
	{
		commandList.push_back(std::make_shared<MoveDown>());
	}
	else if (dist.y > 1.0f)
	{
		commandList.push_back(std::make_shared<MoveUp>());
	}

	return commandList;
}

CommandList SeekBehavior::moveInSearchPattern(StrongTransformComponentPtr thisActorTransformComponent)
{
	Vector2D<float> startingPos = thisActorTransformComponent->GetPosition();
	CommandList actions = moveToPosition(startingPos, m_searchPositions[m_currentSearchPos]);

	// Check if the actor didn't move, increment search target
	if (actions.empty()) 
		/*|| (startingPos.x - thisActorTransformComponent->GetPosition().x < .1f 
			&& startingPos.y - thisActorTransformComponent->GetPosition().y < .1f))*/
	{
		m_currentSearchPos = RandomNumberGenerator::GetIntWithinRange(0, m_searchPositions.size() - 1);
	}

    return actions;
}