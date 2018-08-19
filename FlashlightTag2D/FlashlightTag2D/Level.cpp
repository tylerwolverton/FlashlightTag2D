#include "Level.h"
#include "Shader.h"

Level::Level(int levelWidth, int levelHeight, std::string spritePath, std::string vertexShader, std::string fragmentShader)
	: m_levelSize(Vector2D<int>(levelWidth, levelHeight)),
	  m_spritePath(spritePath),
	  m_shader(std::make_shared<Shader>(vertexShader, fragmentShader))
{
}

Level::~Level()
{
}