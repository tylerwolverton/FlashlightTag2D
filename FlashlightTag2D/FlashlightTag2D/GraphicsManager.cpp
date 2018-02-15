#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "GameStateComponent.h"
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
	auto cameraTransformComponent = currentCamera->GetTransformComponent();
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	auto cameraPos = cameraTransformComponent->GetPosition();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

    GLfloat light[3] = { 0.0, 0.0, 0.0 };
    GLfloat lightDir[2] = { 0.0, 0.0 };
    GLfloat lightPos[2] = { 0.0, 0.0 };
	for (auto actor : gameActors)
	{
        auto graphicsComponent = actor->GetGraphicsComponent();
        if (graphicsComponent == nullptr)
        {
            continue;
        }

        auto actorTransformComponent = actor->GetTransformComponent();
        if (actorTransformComponent == nullptr)
        {
            continue;
        }      

        auto actorPos = actorTransformComponent->GetPosition();
		auto actorSize = actorTransformComponent->GetSize();

		// Prepare transformations
		m_shader.UseProgram();
		Matrix4<GLfloat> model;
        auto actorLocation = actorPos - cameraPos - graphicsComponent->GetImageOffset();
		model = model.Translate(actorLocation);

        // Temporary code to test lighting
        auto gameStateComponent = actor->GetGameStateComponent();
        if (gameStateComponent != nullptr
            && gameStateComponent->GetName() == "Player")
        {
            light[0] = actorLocation.x; 
            light[1] = actorLocation.y;
            light[2] = 250.0f;

            auto normalizedVelocity = actor->GetPhysicsComponent()->GetVelocity().Normalize();
            //Vector2D<float> normalizedVelocity(1.0, 0.0); //actor->GetPhysicsComponent()->GetVelocity().Normalize();
            lightDir[0] = -actorTransformComponent->GetDirection().x;
            lightDir[1] = -actorTransformComponent->GetDirection().y;

            lightPos[0] = actorLocation.x + actorTransformComponent->GetSize().x / 2;
            lightPos[1] = actorLocation.y + actorTransformComponent->GetSize().y / 2;
        }
        
        m_shader.SetVec3("lightSrc", light);
        m_shader.SetVec2("lightDir", lightDir);
        m_shader.SetVec2("lightPos", lightPos);
		//model.Print();
		//m_projMatrix->Print();

		//(model * *(m_projMatrix.get())).Print();
		/*model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		*/

        // TODO: find a better scaling method
		model = model.Scale(actorSize);
		
		//Vector2D<GLfloat> texturePos(1, 1);

		m_shader.SetMatrix4("model", model.GetPtrToFlattenedData().get());
		m_shader.SetVec2("textureSize", graphicsComponent->GetTextureSize().GetPtrToFlattenedData().get());
		m_shader.SetVec2("texturePos", graphicsComponent->GetTexturePos().GetPtrToFlattenedData().get());
		//this->m_shader.SetVector3f("spriteColor", color);
		
		glActiveTexture(GL_TEXTURE0);
		
		graphicsComponent->GetTexture().BindTexture();

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