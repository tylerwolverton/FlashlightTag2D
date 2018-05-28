#include "HideBehavior.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "Vector2D.h"
#include "Command.h"
#include "GameTypes.h"

HideBehavior::HideBehavior()
{
}

HideBehavior::~HideBehavior()
{
}

std::vector<std::shared_ptr<Command>> HideBehavior::Update(const GameActor& thisActor)
{
    //  Look for seeker in light cone
    // Get list of nearby actors
    auto thisActorTransformComponent = thisActor.GetTransformComponent();
    if (thisActorTransformComponent == nullptr)
    {
        return std::vector<std::shared_ptr<Command>>();
    }

	std::vector<std::shared_ptr<GameActor>> actorList = ServiceLocator::GetActorFactory()->GetActorList();

    std::vector<std::shared_ptr<TransformComponent>> seekerTransformComponents;
    for (auto otherActor : actorList)
    {
        auto gameStateComponent = otherActor->GetGameStateComponent();
        if (gameStateComponent == nullptr)
        {
            continue;
        }

        if (gameStateComponent->GetRole() == EGameRole::Seeker)
        {
            auto otherActorTransformComponent = otherActor->GetTransformComponent();
            if (otherActorTransformComponent == nullptr)
            {
                continue;
            }

            auto distToOtherActor = (otherActorTransformComponent->GetPosition() - thisActorTransformComponent->GetPosition()).Length();
            if (distToOtherActor < 120.0f)
            {
                seekerTransformComponents.push_back(otherActorTransformComponent);
            }
        }
    }

    //   Seeker found - run away
    if (seekerTransformComponents.size() != 0)
    {
        return RunFromSeekers(thisActorTransformComponent, seekerTransformComponents);
    }

    //   Seeker not found - stay or change hiding space
    //actor.SetCommands(std::make_shared<std::vector<std::shared_ptr<Command>>>(Hide()));

	return std::vector<std::shared_ptr<Command>>();
}

std::vector<std::shared_ptr<Command>> HideBehavior::RunFromSeekers(const std::shared_ptr<TransformComponent> thisActorTransformComponent, const std::vector<std::shared_ptr<TransformComponent>>& seekerTransformComponents)
{
    Vector2D<float> totalDistance(0, 0);
    for (auto comp : seekerTransformComponents)
    {
        totalDistance -= comp->GetPosition() - thisActorTransformComponent->GetPosition();
    }

    std::vector<std::shared_ptr<Command>> commands;
    
    if (totalDistance.x < -1.0f)
    {
        commands.push_back(std::make_shared<MoveLeft>());
    }
    else if (totalDistance.x > 1.0f)
    {
        commands.push_back(std::make_shared<MoveRight>());
    }

    if (totalDistance.y < -1.0f)
    {
        commands.push_back(std::make_shared<MoveDown>());
    }
    else if (totalDistance.y > 1.0f)
    {
        commands.push_back(std::make_shared<MoveUp>());
    }

    return commands;
}