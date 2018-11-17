#include "RushBehavior.h"
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

RushBehavior::RushBehavior()
    : m_chaseActor(false)
{
    m_targetActorPtr = ServiceLocator::GetActorFactory()->GetPlayer();
}

RushBehavior::~RushBehavior()
{
}

std::vector<std::shared_ptr<Command>> RushBehavior::Update(const GameActor& thisActor)
{
    // Run towards target
    auto thisActorTransformCompPtr = thisActor.GetTransformCompPtr();
    if (thisActorTransformCompPtr == nullptr)
    {
        return std::vector<std::shared_ptr<Command>>();
    }
        
    auto targetActorTransformCompPtr = m_targetActorPtr->GetTransformCompPtr();
    if (targetActorTransformCompPtr == nullptr)
    {
        return std::vector<std::shared_ptr<Command>>();
    }

    float speed = 1.0;
    auto thisActorPhysicsCompPtr = thisActor.GetPhysicsCompPtr();
    if (thisActorPhysicsCompPtr != nullptr)
    {
        speed = thisActorPhysicsCompPtr->GetCurSpeed();
    }

    return moveTowardsTarget(thisActorTransformCompPtr, targetActorTransformCompPtr, speed);
}

std::vector<std::shared_ptr<Command>> RushBehavior::moveTowardsTarget(const std::shared_ptr<TransformComponent>& thisActorTransformCompPtr,
                                                                      const std::shared_ptr<TransformComponent>& targetActorTransformCompPtr, 
                                                                      float speed)
{
    return moveToPosition(thisActorTransformCompPtr->GetPosition(), targetActorTransformCompPtr->GetPosition(), speed);
}