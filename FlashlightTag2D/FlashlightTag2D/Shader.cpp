#pragma once

/*
	From: http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/
*/

#include "Shader.h"

Shader::Shader(std::string vertexShader, std::string fragmentShader)
	: m_vertexShaderStr(vertexShader),
	  m_fragmentShaderStr(fragmentShader)
{
}

Shader::~Shader()
{
	/* Cleanup all the things we bound and allocated */
	glUseProgram(0);
	glDetachShader(m_shaderProgram, m_vertexshader);
	glDetachShader(m_shaderProgram, m_fragmentShader);

	glDeleteProgram(m_shaderProgram);

	glDeleteShader(m_vertexshader);
	glDeleteShader(m_fragmentShader);
}

bool Shader::Init()
{
	// Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
	m_shaderProgram = glCreateProgram();
	
	if (!loadVertexShader(m_vertexShaderStr))
		return false;

	if (!loadFragmentShader(m_fragmentShaderStr))
		return false;

	// All shaders has been created, now we must put them together into one large object
	return linkShaders();
}

std::string Shader::readFile(const char* file)
{
	// Open file
	std::ifstream t(file);

	// Read file into buffer
	std::stringstream buffer;
	buffer << t.rdbuf();

	// Make a std::string and fill it with the contents of buffer
	std::string fileContent = buffer.str();

	return fileContent;
}

bool Shader::loadVertexShader(const std::string &filename)
{
	std::cout << "Linking Vertex shader" << std::endl;

	// Read file as std::string 
	std::string str = readFile(filename.c_str());

	// c_str() gives us a const char*, but we need a non-const one
	char* src = const_cast<char*>(str.c_str());
	int32_t size = str.length();

	// Create an empty vertex shader handle
	m_vertexshader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to OpenGL
	glShaderSource(m_vertexshader, 1, &src, &size);

	// Compile the vertex shader
	glCompileShader(m_vertexshader);

	int wasCompiled = 0;
	glGetShaderiv(m_vertexshader, GL_COMPILE_STATUS, &wasCompiled);

	if (wasCompiled == 0)
	{
		printShaderCompilationErrorInfo(m_vertexshader);
		return false;
	}

	glAttachShader(m_shaderProgram, m_vertexshader);
	return true;
}

bool Shader::loadFragmentShader(const std::string &filename)
{
	std::cout << "Loading Fragment Shader" << std::endl;

	// Read file as std::string 
	std::string str = readFile(filename.c_str());

	// c_str() gives us a const char*, but we need a non-const one
	char* src = const_cast<char*>(str.c_str());
	int32_t size = str.length();

	// Create an empty vertex shader handle
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the vertex shader source code to OpenGL
	glShaderSource(m_fragmentShader, 1, &src, &size);

	// Compile the vertex shader
	glCompileShader(m_fragmentShader);

	int wasCompiled = 0;
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &wasCompiled);

	if (wasCompiled == false)
	{
		printShaderCompilationErrorInfo(m_fragmentShader);
		return false;
	}

	glAttachShader(m_shaderProgram, m_fragmentShader);
	return true;
}

bool Shader::linkShaders()
{
	// Link. At this point, our shaders will be inspected/optized and the binary code generated
	// The binary code will then be uploaded to the GPU
	glLinkProgram(m_shaderProgram);

	// Verify that the linking succeeded
	int isLinked;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == false)
		printShaderLinkingError(m_shaderProgram);

	return isLinked != 0;
}

void Shader::printShaderLinkingError(int32_t shaderId)
{
	std::cout << "=======================================\n";
	std::cout << "Shader linking failed : " << std::endl;

	// Find length of shader info log
	int maxLength;
	glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	std::cout << "Info Length : " << maxLength << std::endl;

	// Get shader info log
	char* shaderProgramInfoLog = new char[maxLength];
	glGetProgramInfoLog(m_shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

	std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

	/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
	/* In this simple program, we'll just leave */
	delete shaderProgramInfoLog;
	return;
}

// If something went wrong whil compiling the shaders, we'll use this function to find the error
void Shader::printShaderCompilationErrorInfo(int32_t shaderId)
{
	std::cout << "=======================================\n";
	std::cout << "Shader compilation failed : " << std::endl;

	// Find length of shader info log
	int maxLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

	// Get shader info log
	char* shaderInfoLog = new char[maxLength];
	glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog);

	// Print shader info log
	std::cout << "\tError info : " << shaderInfoLog << std::endl;

	std::cout << "=======================================\n\n";
	delete shaderInfoLog;
}