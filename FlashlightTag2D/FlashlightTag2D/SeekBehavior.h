#pragma once
#include "Behavior.h"
#include "Types.h"
#include "Vector2D.h"

class SeekBehavior :
	public Behavior
{
public:
	SeekBehavior(const Vector2D<int>& levelSize);
	virtual ~SeekBehavior();

	virtual CommandList Update(const GameActor& thisActor) override;

private:
    CommandList moveTowardsTarget(StrongTransformComponentPtr thisActorTransformComponent,
                                  StrongTransformComponentPtr targetActorTransformComponent);
    CommandList moveInSearchPattern(StrongTransformComponentPtr thisActorTransformComponent);

	CommandList SeekBehavior::moveToPosition(Vector2D<float> currentPos,
											 Vector2D<float> targetPos);

	void initSearchPositions();

    StrongGameActorPtr m_targetActor;

    enum EState
    {
        Search,
        Chase
    };
    EState m_currState;

    float m_maxChaseDistance;
    float m_minTagDistance;

	std::vector<Vector2D<float>> m_searchPositions;
	int m_currentSearchPos;
	int m_ticksSinceLastMove;
	Vector2D<int> m_levelSize;
};

