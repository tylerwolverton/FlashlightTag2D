#include "Level.h"
#include "Shader.h"
#include "GameActor.h"
#include "TransformComponent.h"

Level::Level(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
    : m_levelSizeVec(Vector2D<int>(levelWidth, levelHeight)),
      m_spritePath(spritePath),
      m_shaderPtr(std::make_shared<Shader>(vertexShader, fragmentShader))
{
}

Level::Level(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader)
    : m_levelSizeVec(Vector2D<int>(levelWidth, levelHeight)),
      m_tileVec(tileVec),
      m_shaderPtr(std::make_shared<Shader>(vertexShader, fragmentShader))
{
}

Level::~Level()
{
}

void Level::AddActorToTiles(std::shared_ptr<GameActor> actor)
{
    auto transformCompPtr = actor->GetTransformComponent();
    auto physicsCompPtr = actor->GetPhysicsComponent();
    if (transformCompPtr == nullptr
        || physicsCompPtr == nullptr) // only care about actors with physics component
    {
        return;
    }

    ActorId actorId = actor->GetActorId();
    auto actorIter = m_actorToTilesMap.find(actorId);
    if (actorIter != m_actorToTilesMap.end())
    {
        // remove actor from its current game tiles
        for (auto tile : actorIter->second)
        {
            tile->RemoveActor(actorId);
        }
        actorIter->second.clear();
    }

    std::vector<std::shared_ptr<GameTile>> newTileVec = getTilesUnderActor(transformCompPtr);

    m_actorToTilesMap.insert(std::make_pair(actorId, newTileVec));
    
    actor->SetTileVec(newTileVec);
    // add actor to each of its game tiles
    for (auto tile : newTileVec)
    {
        tile->AddActor(actor);
    }
}

std::vector<std::shared_ptr<GameTile>> Level::getTilesUnderActor(std::shared_ptr<TransformComponent> transformComp)
{
    Vector2D<float> actorPos = transformComp->GetPosition();
    Vector2D<float> actorSize = transformComp->GetSize();

    Vector2D<float> topLeft(actorPos - actorSize / 2);
    Vector2D<int> topLeftTileIdx(topLeft.x / TILE_WIDTH, topLeft.y / TILE_HEIGHT);

    Vector2D<float> bottomRight(actorPos + actorSize / 2);
    Vector2D<int> bottomRightTileIdx(bottomRight.x / TILE_WIDTH, bottomRight.y / TILE_HEIGHT);

    std::vector<std::shared_ptr<GameTile>> tileVec;
    for (int i = topLeftTileIdx.y; i < bottomRightTileIdx.y; i++)
    {
        for (int j = topLeftTileIdx.x; j < bottomRightTileIdx.x; j++)
        {
            tileVec.push_back(m_tileVec[i][j]);
        }
    }

    return tileVec;
}