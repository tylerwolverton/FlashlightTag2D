#pragma once
#include <string>

class Texture2D
{
public:
	Texture2D(std::string texturePath);
	~Texture2D();
	
	void BindTexture();

private:
	void loadTexture(std::string texturePath);

	unsigned int m_texture;
};

