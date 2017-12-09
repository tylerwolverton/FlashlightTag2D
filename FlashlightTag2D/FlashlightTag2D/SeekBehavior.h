#pragma once
#include "Behavior.h"
#include "Types.h"

class SeekBehavior :
	public Behavior
{
public:
	SeekBehavior();
	virtual ~SeekBehavior();

	virtual CommandList Update(GameActor& thisActor) override;

private:
    CommandList moveTowardsTarget(StrongTransformComponentPtr thisActorTransformComponent,
                                  StrongTransformComponentPtr targetActorTransformComponent);
    CommandList moveInSearchPattern();
    CommandList tagTarget();

    StrongGameActorPtr m_targetActor;

    enum EState
    {
        Search,
        Chase
    };
    EState m_currState;

    float m_maxChaseDistance;
    float m_minTagDistance;
};

