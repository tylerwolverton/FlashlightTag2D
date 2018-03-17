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

CommandList HideBehavior::Update(const GameActor& thisActor)
{
    //  Look for seeker in light cone
    // Get list of nearby actors
    auto thisActorTransformComponent = thisActor.GetTransformComponent();
    if (thisActorTransformComponent == nullptr)
    {
        return CommandList();
    }

    auto actorList = ServiceLocator::GetActorFactory()->GetActorList();

    std::vector<StrongTransformComponentPtr> seekerTransformComponents;
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
            if (distToOtherActor < 300.0f)
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
    //actor.SetCommands(std::make_shared<CommandList>(Hide()));

	return CommandList();
}

CommandList HideBehavior::RunFromSeekers(const StrongTransformComponentPtr thisActorTransformComponent, const std::vector<StrongTransformComponentPtr>& seekerTransformComponents)
{
    Vector2D<float> totalDistance(0, 0);
    for (auto comp : seekerTransformComponents)
    {
        totalDistance -= comp->GetPosition() - thisActorTransformComponent->GetPosition();
    }

    CommandList commandList;
    
    if (totalDistance.x < -1.0f)
    {
        commandList.push_back(std::make_shared<MoveLeft>());
    }
    else if (totalDistance.x > 1.0f)
    {
        commandList.push_back(std::make_shared<MoveRight>());
    }

    if (totalDistance.y < -1.0f)
    {
        commandList.push_back(std::make_shared<MoveDown>());
    }
    else if (totalDistance.y > 1.0f)
    {
        commandList.push_back(std::make_shared<MoveUp>());
    }

    return commandList;
}