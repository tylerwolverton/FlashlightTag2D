#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "Vector2D.h"
#include "Texture2D.h"
//#include "stb_image.h"

GraphicsManager::GraphicsManager(SDL_Window* window)
	: m_window(window)
{
	setOpenGLAttributes();

	// Create our opengl context and attach it to our window
	m_mainContext = SDL_GL_CreateContext(m_window);

	// This makes our buffer swap synchronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	if (m_shader.Init())
	{
		m_shader.UseProgram();
	}
	
	initializeRenderData();

	int windowWidth, windowHeight;
	SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
	m_projMatrix = Matrix4<GLfloat>::CreateOrthoMatrix(0, windowWidth, windowHeight, 0, -1, 1);
	m_shader.SetMatrix4("projection", m_projMatrix->GetPtrToFlattenedData().get());
	m_shader.SetInt("image", 0);
}

GraphicsManager::~GraphicsManager()
{
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &m_quadVAO);

	// Delete our OpengL context
	SDL_GL_DeleteContext(m_mainContext);
}

bool GraphicsManager::setOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards should be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

bool GraphicsManager::initializeRenderData()
{
	// Configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->m_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->m_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void GraphicsManager::Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera)
{
	StrongActorComponentPtr cameraTransformComponent = currentCamera->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawCameraTransformComponent = std::dynamic_pointer_cast<TransformComponent>(cameraTransformComponent);
	auto cameraPos = rawCameraTransformComponent->GetPosition();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto actor : gameActors)
	{
		StrongActorComponentPtr graphicsComponent = actor->GetComponentByName(EComponentNames::GraphicsComponentEnum);
		if (graphicsComponent == nullptr)
		{
			continue;
		}

		StrongActorComponentPtr actorTransformComponent = actor->GetComponentByName(EComponentNames::TransformComponentEnum);
		if (actorTransformComponent == nullptr)
		{
			continue;
		}

		std::shared_ptr<GraphicsComponent> rawGraphicsComponent = std::dynamic_pointer_cast<GraphicsComponent>(graphicsComponent);
		std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actorTransformComponent);
		auto actorPos = rawActorTransformComponent->GetPosition();
		auto actorSize = rawActorTransformComponent->GetSize();

		// Prepare transformations
		m_shader.UseProgram();
		Matrix4<GLfloat> model;
		model = model.Translate(actorPos - cameraPos - rawGraphicsComponent->GetImageOffset());
		//model.Print();
		//m_projMatrix->Print();

		//(model * *(m_projMatrix.get())).Print();
		/*model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		*/
		model = model.Scale(actorSize);
		
		//Vector2D<GLfloat> texturePos(1, 1);

		m_shader.SetMatrix4("model", model.GetPtrToFlattenedData().get());
		m_shader.SetVec2("textureSize", rawGraphicsComponent->GetTextureSize().GetPtrToFlattenedData().get());
		m_shader.SetVec2("texturePos", rawGraphicsComponent->GetTexturePos().GetPtrToFlattenedData().get());
		//this->m_shader.SetVector3f("spriteColor", color);
		
		glActiveTexture(GL_TEXTURE0);
		
		rawGraphicsComponent->GetTexture().BindTexture();

		glBindVertexArray(this->m_quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	SDL_GL_SwapWindow(m_window);
}

void GraphicsManager::ClearScreen()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(m_window);
}