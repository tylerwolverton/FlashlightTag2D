#include "Texture2D.h"
#include "stb_image.h"
#include <glew.h>
#include <iostream>

Texture2D::Texture2D()
{
}

Texture2D::Texture2D(std::string texturePath)
{
    loadTexture(texturePath);
}

Texture2D::~Texture2D()
{
}

void Texture2D::loadTexture(std::string texturePath)
{
    // Texture code
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        m_textureWidth = width;
        m_textureHeight = height;

        //std::cout << "Successfully loaded texture" << std::endl;
    }
    else
    {
        //std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture2D::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}