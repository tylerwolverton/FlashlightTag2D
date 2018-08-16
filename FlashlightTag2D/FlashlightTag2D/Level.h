#pragma once
#include "Vector2D.h"

#include <string>

class Shader;

class Level
{
public:
	Level(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader);
	virtual ~Level();

	Vector2D<int> GetLevelSize() { return m_levelSize; }
	std::shared_ptr<Shader> GetShader() { return m_shader; }
	std::string GetSpritePath() { return m_spritePath; }
	
	void RunShaders();

private:
	Vector2D<int> m_levelSize;
	std::shared_ptr<Shader> m_shader;
	std::string m_spritePath;
};

