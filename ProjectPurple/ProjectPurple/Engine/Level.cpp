#include "Level.h"
#include "Shader.h"
#include "GameActor.h"
#include "TransformComponent.h"

Level::Level(int levelWidth, int levelHeight, 
             const std::string& spritePath, 
             const std::string& vertexShader, const std::string& fragmentShader)
    : m_levelSizeVec(Vector2D<int>(levelWidth, levelHeight)),
      m_spritePath(spritePath),
      m_shaderPtr(std::make_shared<Shader>(vertexShader, fragmentShader))
{
}

Level::Level(int levelWidth, int levelHeight, 
             const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr, 
             const std::string& vertexShader, const std::string& fragmentShader)
    : m_levelSizeVec(Vector2D<int>(levelWidth, levelHeight)),
      m_tilePtrVecVecPtr(tilePtrVecVecPtr),
      m_shaderPtr(std::make_shared<Shader>(vertexShader, fragmentShader))
{
}

Level::~Level()
{
}

void Level::AddActorToTiles(const std::shared_ptr<GameActor>& actor)
{
    auto transformCompPtr = actor->GetTransformCompPtr();
    auto physicsCompPtr = actor->GetPhysicsCompPtr();
    if (transformCompPtr == nullptr
        || physicsCompPtr == nullptr) // only care about actors with physics component
    {
        return;
    }

    ActorId actorId = actor->GetActorId();
    auto actorIter = m_actorToTilesMapPtr->find(actorId);
    if (actorIter != m_actorToTilesMapPtr->end())
    {
        // remove actor from its current game tiles
        for (auto tile : actorIter->second)
        {
            tile->RemoveActor(actorId);
        }
        actorIter->second.clear();
    }

    std::vector<std::shared_ptr<GameTile>> newTilePtrVec = getTilesUnderActor(transformCompPtr);

    m_actorToTilesMapPtr->insert(std::make_pair(actorId, newTilePtrVec));
    
    actor->SetTileVec(newTilePtrVec);
    // add actor to each of its game tiles
    for (auto tile : newTilePtrVec)
    {
        tile->AddActor(actor);
    }
}

std::vector<std::shared_ptr<GameTile>> Level::getTilesUnderActor(const std::shared_ptr<TransformComponent>& transformCompPtr) const
{
    Vector2D<float> actorPos = transformCompPtr->GetPosition();
    Vector2D<float> actorSize = transformCompPtr->GetSize();

    Vector2D<float> topLeft(actorPos - actorSize / 2);
    Vector2D<int> topLeftTileIdx(topLeft.x / TILE_WIDTH, topLeft.y / TILE_HEIGHT);

    Vector2D<float> bottomRight(actorPos + actorSize / 2);
    Vector2D<int> bottomRightTileIdx(bottomRight.x / TILE_WIDTH, bottomRight.y / TILE_HEIGHT);

    std::vector<std::shared_ptr<GameTile>> tileVec;
    for (int i = topLeftTileIdx.y; i < bottomRightTileIdx.y; i++)
    {
        for (int j = topLeftTileIdx.x; j < bottomRightTileIdx.x; j++)
        {
            tileVec.push_back((*m_tilePtrVecVecPtr)[i][j]);
        }
    }

    return tileVec;
}