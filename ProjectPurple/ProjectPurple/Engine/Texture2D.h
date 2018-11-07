#pragma once
#include <string>

class Texture2D
{
public:
    Texture2D();
    Texture2D(std::string texturePath);
    ~Texture2D();
    
    void BindTexture();
    int GetWidth() { return m_textureWidth; }
    int GetHeight() { return m_textureHeight; }

private:
    void loadTexture(std::string texturePath);

    unsigned int m_texture;
    int m_textureWidth;
    int m_textureHeight;
};

