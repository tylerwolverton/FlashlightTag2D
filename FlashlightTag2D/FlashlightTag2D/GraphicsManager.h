#pragma once
#include "Types.h"

#include <glew.h>
#include "Matrix4.h"
#include "Shader.h"
#include "Texture2D.h"

#define GL3_PROTOTYPES 1

struct SDL_Window;

class GraphicsManager
{
public:
	GraphicsManager(SDL_Window* window);
	~GraphicsManager();

	void Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera);
	void ClearScreen();

    void SetBackgroundTexture(std::string texturePath) { m_backgroundTexture = std::make_shared<Texture2D>(texturePath); };

private:
	SDL_Window* m_window;

	// Our opengl context handle
	SDL_GLContext m_mainContext;
	
	Shader m_shader;
	GLuint VBO, m_quadVAO;
	
	std::unique_ptr<Matrix4<GLfloat>> m_projMatrix;

    StrongTexture2DPtr m_backgroundTexture;

	bool setOpenGLAttributes();
	bool initializeRenderData();
    void renderBackground(Vector2D<float> cameraPos);
};

