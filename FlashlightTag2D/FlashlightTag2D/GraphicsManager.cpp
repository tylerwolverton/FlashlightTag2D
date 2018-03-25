#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "GameActor.h"
#include "Vector2D.h"

GraphicsManager::GraphicsManager(SDL_Window* window)
	: m_window(window),
	  m_lastComponentId(0)
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
	m_projMatrix = Matrix4<GLfloat>::CreateOrthoMatrix(0, (float)windowWidth, (float)windowHeight, 0, -1, 1);
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

void GraphicsManager::AddGraphicsComponent(GraphicsComponent comp) 
{ 
    m_graphicsComponentVec.push_back(comp); 
};

int GraphicsManager::AddGraphicsComponent(std::string texturePath, int animationTimer, StrongTransformComponentPtr transformComponent)
{
	int compId = getNextComponentId();
	m_graphicsComponentVec.emplace_back(compId, texturePath, animationTimer, transformComponent);
	return compId;
}

GraphicsComponent* GraphicsManager::GetComponentById(int id)
{
	return &m_graphicsComponentVec[id];
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

void GraphicsManager::UpdateComponents()
{
	for (auto comp : m_graphicsComponentVec)
	{
		comp.Update();
	}
}

void GraphicsManager::AddCamera(StrongGameActorPtr camera)
{
    if (m_pCurrentCamera == nullptr)
    {
        m_pCurrentCamera = camera;
    }

    m_pCameraList.push_back(camera);
}

void GraphicsManager::Render()
{
	auto cameraTransformComponent = m_pCurrentCamera->GetTransformComponent();
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	auto cameraPos = cameraTransformComponent->GetPosition();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
    
    // opengl likes flat arrays...
    std::vector<GLfloat> lightVec;
    std::vector<GLfloat> lightDirVec;
    std::vector<GLfloat> lightPosVec;
    for (auto graphicsComponent : m_graphicsComponentVec)
    {
        auto actorTransformComponent = *(graphicsComponent.GetTransformComponent());

        auto actorPos = actorTransformComponent.GetPosition();
        auto actorSize = actorTransformComponent.GetSize();
        auto actorLocation = actorPos - cameraPos - graphicsComponent.GetImageOffset();
        
        lightVec.push_back(actorLocation.x); lightVec.push_back(actorLocation.y); lightVec.push_back(250.0f);

        lightDirVec.push_back(actorTransformComponent.GetDirection().x);
        lightDirVec.push_back(actorTransformComponent.GetDirection().y);

        lightPosVec.push_back(actorLocation.x + actorTransformComponent.GetSize().x / 2);
        lightPosVec.push_back(actorLocation.y + actorTransformComponent.GetSize().y / 2);
    }

    m_shader.SetVec3("lightSrc", &lightVec.front(), lightVec.size() / 3);
    m_shader.SetVec2("lightDir", &lightDirVec.front(), lightDirVec.size() / 2);
    m_shader.SetVec2("lightPos", &lightPosVec.front(), lightPosVec.size() / 2);

    renderBackground(cameraPos);

	for (auto graphicsComponent : m_graphicsComponentVec)
	{
        auto actorTransformComponent = *(graphicsComponent.GetTransformComponent());   

        auto actorPos = actorTransformComponent.GetPosition();
		auto actorSize = actorTransformComponent.GetSize();

		// Prepare transformations
		Matrix4<GLfloat> model;
        auto actorLocation = actorPos - cameraPos - graphicsComponent.GetImageOffset();
		model = model.Translate(actorLocation);

        // TODO: find a better scaling method
		model = model.Scale(actorSize);

		m_shader.SetMatrix4("model", model.GetPtrToFlattenedData().get());
		m_shader.SetVec2("textureSize", graphicsComponent.GetTextureSize().GetPtrToFlattenedData().get());
		m_shader.SetVec2("texturePos", graphicsComponent.GetTexturePos().GetPtrToFlattenedData().get());
		
		glActiveTexture(GL_TEXTURE0);
		
		graphicsComponent.GetTexture()->BindTexture();

		glBindVertexArray(this->m_quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	SDL_GL_SwapWindow(m_window);
}

void GraphicsManager::renderBackground(Vector2D<float> cameraPos)
{
    // Render the background
    if (m_backgroundTexture->GetWidth() != 0)
    {

        // Use the whole texture starting from the top left
        Vector2D<GLfloat> textureSize(1.0f, 1.0f);
        Vector2D<GLfloat> texturePos(0, 0);

        Matrix4<GLfloat> backgroundModel;
        backgroundModel = backgroundModel.Translate(-cameraPos);
        
        // Scale by the actual size of the sprite
        Vector2D<GLfloat> spriteSize((float)m_backgroundTexture->GetWidth(), (float)m_backgroundTexture->GetHeight());
        backgroundModel = backgroundModel.Scale(spriteSize);

        m_shader.SetMatrix4("model", backgroundModel.GetPtrToFlattenedData().get());
        m_shader.SetVec2("textureSize", textureSize.GetPtrToFlattenedData().get());
        m_shader.SetVec2("texturePos", texturePos.GetPtrToFlattenedData().get());

        glActiveTexture(GL_TEXTURE0);

        m_backgroundTexture->BindTexture();

        glBindVertexArray(this->m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void GraphicsManager::ClearScreen()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(m_window);
}