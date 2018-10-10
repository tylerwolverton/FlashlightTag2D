#pragma once
#include "Vector2D.h"
#include "Types.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

class GraphicsComponent;
class Shader;

class Level
{
public:
	Level(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
    Level(int levelWidth, int levelHeight, std::vector<std::vector<int>> tileVec, std::string vertexShader, std::string fragmentShader);
	virtual ~Level();

	Vector2D<int> GetLevelSize() { return m_levelSize; }
	std::shared_ptr<Shader> GetShader() { return m_shader; }
	std::string GetSpritePath() { return m_spritePath; }
    std::vector<std::vector<int>> GetTileVec() { return m_tileVec; }
	
	virtual void PrepShaders(std::map<ComponentId, std::shared_ptr<GraphicsComponent>> graphicsComponentPtrMap, Vector2D<float> cameraPos) = 0;
    virtual void SetupLevel() {};

protected:
	Vector2D<int> m_levelSize;
	std::shared_ptr<Shader> m_shader;
	std::string m_spritePath;
    std::vector<std::vector<int>> m_tileVec;
};

