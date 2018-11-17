#pragma once
#include "Vector2D.h"
#include "Types.h"
#include "GameTile.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class GameActor;
class TransformComponent;
class GraphicsComponent;
class Shader;

class Level
{
public:
    // only support 32x32 tiles for now
    static const int TILE_WIDTH = 32;
    static const int TILE_HEIGHT = 32;

    Level(int levelWidth, int levelHeight, 
          const std::string& spritePath, 
          const std::string& vertexShader, const std::string& fragmentShader);
    Level(int levelWidth, int levelHeight, 
          const std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>>& tilePtrVecVecPtr, 
          const std::string& vertexShader, const std::string& fragmentShader);
    virtual ~Level();

    Vector2D<int> GetLevelSize() const { return m_levelSizeVec; }
    std::shared_ptr<Shader> GetShader() const { return m_shaderPtr; }
    std::string GetSpritePath() const { return m_spritePath; }

    std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>> GetTileVec() const { return m_tilePtrVecVecPtr; }
    void AddActorToTiles(const std::shared_ptr<GameActor>& actor);

    virtual void PrepShaders(const std::map<ComponentId, std::shared_ptr<GraphicsComponent>>& graphicsComponentPtrMap, 
                             const Vector2D<float>& cameraPos) = 0;
    virtual void SetupLevel() {};

protected:
    Vector2D<int> m_levelSizeVec;
    std::shared_ptr<Shader> m_shaderPtr;
    std::string m_spritePath;
    std::shared_ptr<std::vector<std::vector<std::shared_ptr<GameTile>>>> m_tilePtrVecVecPtr;

private:
    std::shared_ptr<std::map<ActorId, std::vector<std::shared_ptr<GameTile>>>> m_actorToTilesMapPtr;

    std::vector<std::shared_ptr<GameTile>> getTilesUnderActor(const std::shared_ptr<TransformComponent>& transformCompPtr) const;
};

