#pragma once
#include "Behavior.h"
#include "Vector2D.h"

class SeekBehavior :
	public Behavior
{
public:
	SeekBehavior(const Vector2D<int>& levelSize);
	virtual ~SeekBehavior();

	virtual std::vector<std::shared_ptr<Command>> Update(const GameActor& thisActor) override;

private:
    std::vector<std::shared_ptr<Command>> moveTowardsTarget(std::shared_ptr<TransformComponent> thisActorTransformComponent,
                                  std::shared_ptr<TransformComponent> targetActorTransformComponent);
    std::vector<std::shared_ptr<Command>> moveInSearchPattern(std::shared_ptr<TransformComponent> thisActorTransformComponent);

	std::vector<std::shared_ptr<Command>> SeekBehavior::moveToPosition(Vector2D<float> currentPos,
											 Vector2D<float> targetPos);

	void initSearchPositions();

    std::shared_ptr<GameActor> m_targetActor;

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
	Vector2D<float> m_lastSearchActorPos;
	int m_ticksSinceLastMove;
	int m_maxTicksSinceLastMove;
	Vector2D<int> m_levelSize;

};

