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

	void Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera, SDL_Renderer* renderer);
	void GraphicsManager::RenderBackground(SDL_Texture* sprite, StrongGameActorPtr currentCamera, SDL_Renderer* renderer, int screenWidth, int screenHeight);

	void Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera);
	void ClearScreen();

private:
	SDL_Window* m_window;

	// Our opengl context handle
	SDL_GLContext m_mainContext;

	// Our object has 4 points
	const uint32_t points = 4;

	// Each poin has three values ( x, y, z)
	const uint32_t floatsPerPoint = 2;

	//unsigned int m_texture;
	Shader m_shader;
	GLuint VBO, VAO, EBO, m_quadVAO;

	// The positons of the position and color data within the VAO
	const uint32_t positionAttributeIndex = 0;

	std::unique_ptr<Matrix4<GLfloat>> m_projMatrix;

	bool setOpenGLAttributes();
	bool initializeRenderData();
	bool setupBufferObjects(const std::vector< std::vector<float> > vertices); 
	/*void drawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));*/
};

