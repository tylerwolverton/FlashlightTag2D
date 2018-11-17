#pragma once
#include <string>

class Texture2D
{
public:
    Texture2D();
    Texture2D(const std::string& texturePath);
    ~Texture2D();
    
    void BindTexture() const;
    int GetWidth() const { return m_textureWidth; }
    int GetHeight() const { return m_textureHeight; }

private:
    unsigned int m_texture;
    int m_textureWidth;
    int m_textureHeight;

	void loadTexture(const std::string& texturePath);
};

