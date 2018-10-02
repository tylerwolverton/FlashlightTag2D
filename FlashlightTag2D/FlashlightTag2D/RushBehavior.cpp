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
    m_targetActor = ServiceLocator::GetActorFactory()->GetPlayer();
}

RushBehavior::~RushBehavior()
{
}

std::vector<std::shared_ptr<Command>> RushBehavior::Update(const GameActor& thisActor)
{
    auto thisActorTransformComponent = thisActor.GetTransformComponent();
    if (thisActorTransformComponent == nullptr)
    {
        return std::vector<std::shared_ptr<Command>>();
    }
    	
	auto targetActorTransformComponent = m_targetActor->GetTransformComponent();
	if (targetActorTransformComponent == nullptr)
	{
		return std::vector<std::shared_ptr<Command>>();
	}

    if (m_chaseActor)
    {
        float speed = 1.0;
        auto thisActorPhysicsComponent = thisActor.GetPhysicsComponent();
        if (thisActorPhysicsComponent != nullptr)
        {
            speed = thisActorPhysicsComponent->GetCurSpeed();
        }

        return moveTowardsTarget(thisActorTransformComponent, targetActorTransformComponent, speed);
    }
    else
    {
        if((targetActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition()).Length() < 500)
        {
            m_chaseActor = true;
        }
    }

    return std::vector<std::shared_ptr<Command>>();
}

std::vector<std::shared_ptr<Command>> RushBehavior::moveTowardsTarget(std::shared_ptr<TransformComponent> thisActorTransformComponent,
																	  std::shared_ptr<TransformComponent> targetActorTransformComponent, 
																	  float speed)
{
	return moveToPosition(thisActorTransformComponent->GetPosition(), targetActorTransformComponent->GetPosition(), speed);
}

std::vector<std::shared_ptr<Command>> RushBehavior::moveToPosition(Vector2D<float> currentPos,
																   Vector2D<float> targetPos, 
																   float speed)
{
	std::vector<std::shared_ptr<Command>> commands;

	Vector2D<float> dist = targetPos - currentPos;

	if (dist.x < -speed)
	{
		commands.push_back(std::make_shared<MoveLeft>());
	}
	else if (dist.x > speed)
	{
		commands.push_back(std::make_shared<MoveRight>());
	}

	if (dist.y < -speed)
	{
		commands.push_back(std::make_shared<MoveDown>());
	}
	else if (dist.y > speed)
	{
		commands.push_back(std::make_shared<MoveUp>());
	}

	return commands;
}