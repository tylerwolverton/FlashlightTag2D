#pragma once
#include "Types.h"

#include <SDL.h>
#include <glew.h>
#include <vector>
#include "Matrix4.h"
#include "Shader.h"
#define GL3_PROTOTYPES 1

class GraphicsManager
{
public:
	GraphicsManager(SDL_Window* window);
	~GraphicsManager();

	void Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera);
	void ClearScreen();

private:
	SDL_Window* m_window;

	// Our opengl context handle
	SDL_GLContext m_mainContext;
	
	Shader m_shader;
	GLuint VBO, m_quadVAO;
	
	std::unique_ptr<Matrix4<GLfloat>> m_projMatrix;

	bool setOpenGLAttributes();
	bool initializeRenderData();
};

