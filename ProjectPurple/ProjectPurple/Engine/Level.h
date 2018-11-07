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

    Level(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    Level(int levelWidth, int levelHeight, std::vector<std::vector<std::shared_ptr<GameTile>>> tileVec, std::string vertexShader, std::string fragmentShader);
    virtual ~Level();

    Vector2D<int> GetLevelSize() { return m_levelSize; }
    std::shared_ptr<Shader> GetShader() { return m_shader; }
    std::string GetSpritePath() { return m_spritePath; }
    //std::vector<std::vector<int>> GetTileVec() { return m_tileVec; }
    std::vector<std::vector<std::shared_ptr<GameTile>>> GetTileVec() { return m_tileVec; }
    void AddActorToTiles(std::shared_ptr<GameActor> actor);

    virtual void PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos) = 0;
    virtual void SetupLevel() {};

protected:
    Vector2D<int> m_levelSize;
    std::shared_ptr<Shader> m_shader;
    std::string m_spritePath;
    //std::vector<std::vector<int>> m_tileVec;
    std::vector<std::vector<std::shared_ptr<GameTile>>> m_tileVec;

private:
    std::map<ActorId, std::vector<std::shared_ptr<GameTile>>> m_actorToTilesMap;

    std::vector<std::shared_ptr<GameTile>> getTilesUnderActor(std::shared_ptr<TransformComponent> transformComp);
};

