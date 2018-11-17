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
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    bool Init();

    void UseProgram() const
    {
        // Load the shader into the rendering pipeline
        glUseProgram(m_shaderProgram);
    }

    void SetMatrix4(const std::string& name, const GLfloat* const value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_TRUE, value);
    }

    void SetVec2(const std::string& name, const GLfloat* const value, int size = 1) const
    {
        glUniform2fv(glGetUniformLocation(m_shaderProgram, name.c_str()), size, value);
    }

    void SetVec3(const std::string& name, const GLfloat* const value, int size = 1) const
    {
        glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), size, value);
    }

    void SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
    }

private:
    // The handle to our shader program
    GLuint m_shaderProgram;

    // The handles to the induvidual shader
    GLuint m_vertexshader, m_fragmentShader;

    std::string m_vertexShaderStr, m_fragmentShaderStr;

	std::string readFile(const char* file) const;
	bool loadVertexShader(const std::string& filename);
	bool loadFragmentShader(const std::string& filename);
	bool linkShaders() const;
	void printShaderLinkingError(int32_t shaderId) const;

	// If something went wrong while compiling the shaders, we'll use this function to find the error
	void printShaderCompilationErrorInfo(int32_t shaderId) const;
};
