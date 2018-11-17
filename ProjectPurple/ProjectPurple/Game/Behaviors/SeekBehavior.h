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
	enum EState
	{
		Search,
		Chase
	};
	EState m_currState;

	float m_maxChaseDistance;
	float m_minTagDistance;

	std::shared_ptr<GameActor> m_targetActorPtr;
	std::vector<Vector2D<float>> m_searchPositions;
	int m_currentSearchPos;
	Vector2D<float> m_lastSearchActorPos;
	int m_ticksSinceLastMove;
	int m_maxTicksSinceLastMove;
	Vector2D<int> m_levelSize;

    void initSearchPositions(const std::shared_ptr<TransformComponent>& transformCompPtr);
    
    std::vector<std::shared_ptr<Command>> moveTowardsTarget(const std::shared_ptr<TransformComponent>& thisActorTransformCompPtr,
                                                            const std::shared_ptr<TransformComponent>& targetActorTransformCompPtr,
                                                            float speed);

    std::vector<std::shared_ptr<Command>> moveInSearchPattern(const std::shared_ptr<TransformComponent>& thisActorTransformCompPtr, float speed);
};

