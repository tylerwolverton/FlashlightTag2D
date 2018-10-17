#include "Boss1Behavior.h"
#include "ActorFactory.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"

#include <SDL.h>

Boss1Behavior::Boss1Behavior(std::shared_ptr<GameActor> targetActor, Vector2D<int> levelSize)
    : m_targetActor(targetActor),
      m_levelSize(levelSize),
      m_curState(EBehaviorStates::Move),
      m_lastTickVal(SDL_GetTicks()),
      m_curPosIdx(0)
{
    initializeMovePositions();
}

void Boss1Behavior::initializeMovePositions()
{
    m_movePositionVec.push_back(Vector2D<float>(m_levelSize.x / 2, m_levelSize.y));
    m_movePositionVec.push_back(Vector2D<float>(m_levelSize.x, m_levelSize.y / 2));
    m_movePositionVec.push_back(Vector2D<float>(m_levelSize.x / 2, 0));
    m_movePositionVec.push_back(Vector2D<float>(0, m_levelSize.y / 2));
}

Boss1Behavior::~Boss1Behavior()
{
}

std::vector<std::shared_ptr<Command>> Boss1Behavior::Update(const GameActor& actor)
{
    uint32_t curTicks = SDL_GetTicks();
    auto thisTransformComp = actor.GetTransformComponent();
    auto thisPhysicsComp = actor.GetPhysicsComponent();
    if (thisTransformComp == nullptr
        || thisPhysicsComp == nullptr)
    {
        return std::vector<std::shared_ptr<Command>>();
    }

    Vector2D<float> curPos = thisTransformComp->GetPosition();
    int nextIdx = m_curPosIdx;
    switch (m_curState)
    {
        case EBehaviorStates::Wait:
            if (curTicks - m_lastTickVal > 500)
            {
                m_lastTickVal = curTicks;
                m_curState = EBehaviorStates::Move;
                auto playerTransformComp = m_targetActor->GetTransformComponent();
                if (playerTransformComp != nullptr)
                {
                    m_curPosIdx = (m_curPosIdx + 1) % m_movePositionVec.size();
                }
            }
            break;

        case EBehaviorStates::Move:
            if (curTicks - m_lastTickVal > 500)
            {
                m_lastTickVal = curTicks;
                m_curState = EBehaviorStates::Explode;
                m_bulletCount = 0;
                break;
            }

            return moveToPosition(curPos, m_movePositionVec[nextIdx], 30);

        case EBehaviorStates::Explode:
            if (curTicks - m_lastTickVal > 300)
            {
                m_lastTickVal = curTicks;
                spawnBullets(thisTransformComp);
                m_bulletCount++;
            }
            if (m_bulletCount == 5)
            {
                m_curState = EBehaviorStates::Wait;
            }
            break;
    }

    return std::vector<std::shared_ptr<Command>>();
}

void Boss1Behavior::spawnBullets(std::shared_ptr<TransformComponent> actorTransformComp)
{
    // calculate 8 positions surrounding boss
    auto pos = actorTransformComp->GetPosition();
    auto size = actorTransformComp->GetSize();
    
    auto actorFactory = ServiceLocator::GetActorFactory();
    if (actorFactory == nullptr)
    {
        return;
    }

    std::vector<Vector2D<float>> spawnPosVec;
    spawnPosVec.push_back(Vector2D<float>(pos.x, pos.y - (size.y + 24))); // top
    spawnPosVec.push_back(Vector2D<float>(pos.x + (size.x + 24), pos.y)); // right
    spawnPosVec.push_back(Vector2D<float>(pos.x, pos.y + (size.y + 24))); // bottom
    spawnPosVec.push_back(Vector2D<float>(pos.x - (size.x + 24), pos.y)); // left

    spawnPosVec.push_back((spawnPosVec[0] + spawnPosVec[1]) / 2); // top right
    spawnPosVec.push_back((spawnPosVec[2] + spawnPosVec[1]) / 2); // bottom right
    spawnPosVec.push_back((spawnPosVec[2] + spawnPosVec[3]) / 2); // bottom left
    spawnPosVec.push_back((spawnPosVec[0] + spawnPosVec[3]) / 2); // top left

    spawnPosVec.push_back((spawnPosVec[0] + spawnPosVec[4]) / 2); // top right
    spawnPosVec.push_back((spawnPosVec[4] + spawnPosVec[1]) / 2); // top right

    spawnPosVec.push_back((spawnPosVec[1] + spawnPosVec[5]) / 2); // bottom left
    spawnPosVec.push_back((spawnPosVec[5] + spawnPosVec[2]) / 2); // bottom left

    spawnPosVec.push_back((spawnPosVec[2] + spawnPosVec[6]) / 2); // bottom right
    spawnPosVec.push_back((spawnPosVec[6] + spawnPosVec[3]) / 2); // bottom right

    spawnPosVec.push_back((spawnPosVec[3] + spawnPosVec[7]) / 2); // top left
    spawnPosVec.push_back((spawnPosVec[7] + spawnPosVec[0]) / 2); // top left

    // spawn projectiles
    for (auto spawnPos : spawnPosVec)
    {
        if (spawnPos.x > 0 && spawnPos.x < m_levelSize.x
            && spawnPos.y > 0 && spawnPos.y < m_levelSize.y)
        {
            Vector2D<float> dirVec = (spawnPos - pos).Normalize();
            actorFactory->CreateProjectile(spawnPos, dirVec * 15.0f);
        }
    }
}