#pragma once

/*
    From: http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/
*/

#include <glew.h>
#include <SDL.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream> 

class Shader
{
public:
    Shader(std::string vertexShader, std::string fragmentShader);
    ~Shader();

    bool Init();

    void UseProgram() 
    {
        // Load the shader into the rendering pipeline
        glUseProgram(m_shaderProgram);
    }

    void SetMatrix4(const std::string name, GLfloat* value )
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_TRUE, value);
    }

    void SetVec2(const std::string name, GLfloat* value, int size = 1)
    {
        glUniform2fv(glGetUniformLocation(m_shaderProgram, name.c_str()), size, value);
    }

    void SetVec3(const std::string name, GLfloat* value, int size = 1)
    {
        glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), size, value);
    }

    void SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
    }

private:
    std::string readFile(const char* file);
    bool loadVertexShader(const std::string &filename);
    bool loadFragmentShader(const std::string &filename);
    bool linkShaders();
    void printShaderLinkingError(int32_t shaderId);

    // If something went wrong whil compiling the shaders, we'll use this function to find the error
    void printShaderCompilationErrorInfo(int32_t shaderId);

    // The handle to our shader program
    GLuint m_shaderProgram;

    // The handles to the induvidual shader
    GLuint m_vertexshader, m_fragmentShader;

    std::string m_vertexShaderStr, m_fragmentShaderStr;
};
