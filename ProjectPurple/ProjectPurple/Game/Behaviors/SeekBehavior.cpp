#include "SeekBehavior.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "GameStateComponent.h"
#include "Vector2D.h"
#include "Command.h"
#include "GameTypes.h"
#include "RandomNumberGenerator.h"

#include <random>
#include <algorithm>

SeekBehavior::SeekBehavior(const Vector2D<int>& levelSize)
  : m_levelSize(levelSize),
    m_targetActorPtr(nullptr),
    m_currState(EState::Search),
    m_maxChaseDistance(151.0f),
    m_minTagDistance(100.0f),
    m_currentSearchPos(0),
    m_ticksSinceLastMove(0),
    m_maxTicksSinceLastMove(20),
    m_lastSearchActorPos(Vector2D<float>(-50.0f, -50.0f))
{
}

void SeekBehavior::initSearchPositions(const std::shared_ptr<TransformComponent>& transformCompPtr)
{
    // Add corners
    m_searchPositions.push_back(Vector2D<float>(transformCompPtr->GetSize().x, transformCompPtr->GetSize().y));
    m_searchPositions.push_back(Vector2D<float>(m_levelSize.x - transformCompPtr->GetSize().x, transformCompPtr->GetSize().y));
    m_searchPositions.push_back(Vector2D<float>(transformCompPtr->GetSize().x, m_levelSize.y - transformCompPtr->GetSize().y));
    m_searchPositions.push_back(Vector2D<float>(m_levelSize.x - transformCompPtr->GetSize().x, m_levelSize.y - transformCompPtr->GetSize().y));

    // Add some random nodes in each quadrant
   /* for (int i = 0; i < 3; i++)
    {
        m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(transformCompPtr->GetSize().x, m_levelSize.x / 2),
                                                    RandomNumberGenerator::GetIntWithinRange(transformCompPtr->GetSize().x, m_levelSize.y / 2)));

        m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(m_levelSize.x / 2, m_levelSize.x - transformCompPtr->GetSize().y),
                                                    RandomNumberGenerator::GetIntWithinRange(transformCompPtr->GetSize().x, m_levelSize.y / 2)));

        m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(transformCompPtr->GetSize().x, m_levelSize.x / 2),
                                                    RandomNumberGenerator::GetIntWithinRange(m_levelSize.y / 2, m_levelSize.y - transformCompPtr->GetSize().y)));

        m_searchPositions.push_back(Vector2D<float>(RandomNumberGenerator::GetIntWithinRange(m_levelSize.x / 2, m_levelSize.x - transformCompPtr->GetSize().y),
                                                    RandomNumberGenerator::GetIntWithinRange(m_levelSize.y / 2, m_levelSize.y - transformCompPtr->GetSize().y)));
    }*/

    //m_currentSearchPos = RandomNumberGenerator::GetIntWithinRange(0, m_searchPositions.size() - 1);
}

SeekBehavior::~SeekBehavior()
{
}

std::vector<std::shared_ptr<Command>> SeekBehavior::Update(const GameActor& thisActor)
{
    // Use the current light cone and sound in surrounding area to see if any actors are close, then if a hider is seen, enter the chase state
    // If already chasing and the hider is lost for long enough, enter the search state

    auto thisActorTransformCompPtr = thisActor.GetTransformCompPtr();
    if (thisActorTransformCompPtr == nullptr)
    {
        return std::vector<std::shared_ptr<Command>>();
    }

    if (m_searchPositions.size() == 0)
    {
        initSearchPositions(thisActorTransformCompPtr);
    }

 //   if(m_currState == EState::Search)
 //   {
 //       // Get list of nearby actors
 //       std::vector<std::shared_ptr<GameActor>> actors = ServiceLocator::GetActorFactory()->GetActorList();

 //       // Find closest hider in that list
 //       // Implicitly set max range, variable-ize this later
 //       float distToClosestActor = 200.0f;
 //       for (auto otherActor : actors)
 //       {
    //		auto gameStateComponent = otherActor->GetGameStateCompPtr();
    //		if (gameStateComponent == nullptr)
    //		{
    //			continue;
    //		}

    //		if (//*otherActor != thisActor && 
    //			gameStateComponent->GetRole() == EGameRole::Hider)
    //		{
    //			auto otherActorTransformComponent = otherActor->GetTransformCompPtr();
    //			if (otherActorTransformComponent == nullptr)
    //			{
    //				continue;
    //			}

    //			float distToOtherActor = (otherActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition()).Length();
    //			if (distToOtherActor > 5.0f
    //				&& distToOtherActor < distToClosestActor)
    //			{
    //				m_targetActor = otherActor;
    //				distToClosestActor = distToOtherActor;
    //			}
    //		}
    //	}

    //	if (m_targetActor != nullptr)
    //	{
    //		m_currState = EState::Chase;
    //	}
    //	else
    //	{
    //		float speed = 1.0;
    //		auto thisActorPhysicsComponent = thisActor.GetPhysicsCompPtr();
    //		if (thisActorPhysicsComponent != nullptr)
    //		{
    //			speed = thisActorPhysicsComponent->GetCurSpeed();
    //		}
    //		return moveInSearchPattern(thisActorTransformComponent, speed);
    //	}
    //}
    //else if (m_currState == EState::Chase)
    //{
    //	auto targetActorTransformComponent = m_targetActor->GetTransformCompPtr();
    //	if (targetActorTransformComponent == nullptr)
    //	{
    //		return std::vector<std::shared_ptr<Command>>();
    //	}

    //	float distToTargetActor = (targetActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition()).Length();
    //	if (distToTargetActor < m_minTagDistance)
    //	{
    //		// Tag target
    //		// Set to either seeker or out depending on the game mode
    //		m_targetActor->GetGameStateCompPtr()->SetRole(EGameRole::Seeker);
    //	}
    //	else if (distToTargetActor < m_maxChaseDistance)
    //	{
    //		float speed = 1.0;
    //		auto thisActorPhysicsComponent = thisActor.GetPhysicsCompPtr();
    //		if (thisActorPhysicsComponent != nullptr)
    //		{
    //			speed = thisActorPhysicsComponent->GetCurSpeed();
    //		}
    //		return moveTowardsTarget(thisActorTransformComponent, targetActorTransformComponent, speed);
    //	}

    //	// If we tagged the target or they are too far away, return to search state
    //	m_targetActor = nullptr;
    //	m_currState = EState::Search;
    //}

    // No actions taken
    return std::vector<std::shared_ptr<Command>>();
}

std::vector<std::shared_ptr<Command>> SeekBehavior::moveTowardsTarget(const std::shared_ptr<TransformComponent>& thisActorTransformCompPtr,
                                                                      const std::shared_ptr<TransformComponent>& targetActorTransformCompPtr, 
                                                                      float speed)
{
    return moveToPosition(thisActorTransformCompPtr->GetPosition(), targetActorTransformCompPtr->GetPosition(), speed);
}

std::vector<std::shared_ptr<Command>> SeekBehavior::moveInSearchPattern(const std::shared_ptr<TransformComponent>& thisActorTransformCompPtr, 
                                                                        float speed)
{
    std::vector<std::shared_ptr<Command>> commandPtrVec = moveToPosition(thisActorTransformCompPtr->GetPosition(), m_searchPositions[m_currentSearchPos], speed);

    // Check if the actor didn't move, increment search target
    if ((m_lastSearchActorPos.x - thisActorTransformCompPtr->GetPosition().x < .1f)
        && (m_lastSearchActorPos.y - thisActorTransformCompPtr->GetPosition().y < .1f))
    {
        m_ticksSinceLastMove++;
    }
    m_lastSearchActorPos = thisActorTransformCompPtr->GetPosition();

    if (commandPtrVec.empty() || m_ticksSinceLastMove >= m_maxTicksSinceLastMove)
    {
        m_currentSearchPos = RandomNumberGenerator::GetIntWithinRange(0, m_searchPositions.size() - 1);
        m_ticksSinceLastMove = 0;
    }

    return commandPtrVec;
}