#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "stb_image.h"

GraphicsManager::GraphicsManager(SDL_Window* window)
	: m_window(window)
{
	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	m_mainContext = SDL_GL_CreateContext(m_window);

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	SetupBufferObjects();
}

GraphicsManager::~GraphicsManager()
{
	glDisableVertexAttribArray(0);
	/*glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);*/

	// Delete our OpengL context
	SDL_GL_DeleteContext(m_mainContext);
}

bool GraphicsManager::SetOpenGLAttributes()
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

bool GraphicsManager::SetupBufferObjects()
{
	GLfloat vertices[] = {
		0.25f,  0.25f, 0.0f,  // Top Right
		0.25f, -0.25f, 0.0f,  // Bottom Right
		-0.25f, -0.25f, 0.0f,  // Bottom Left
		-0.25f,  0.25f, 0.0f   // Top Left 
	};

	GLfloat vertices2[] = {
		0.75f,  0.75f, 0.0f,  // Top Right
		0.75f, -0.75f, 0.0f,  // Bottom Right
		-0.75f, -0.75f, 0.0f,  // Bottom Left
		-0.75f,  0.75f, 0.0f   // Top Left 
	};

	GLfloat verticesTex[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	// Generate and assign two Vertex Buffer Objects to our handle
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ..:: Initialization code :: ..
	// 1. Bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTex), verticesTex, GL_STATIC_DRAW);
	// 3. Copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 3. Then set the vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// 4. Unbind VAO (NOT the EBO)
	glBindVertexArray(0);
	
	// Generate and assign a Vertex Array Object to our handle
	/*glGenVertexArrays(1, vao);*/

	// Bind our Vertex Array Object as the current used object
	//glBindVertexArray(vao[0]);

	// Positions
	// ===================
	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	
	//glBufferData(GL_ARRAY_BUFFER, (points * floatsPerPoint) * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable our attribute within the current VAO
	//glEnableVertexAttribArray(positionAttributeIndex);

	// Set up shader ( will be covered in the next part )
	// ===================
	if (!m_shader.Init())
		return false;

	m_shader.UseProgram();

	/*glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	return true;
}

void GraphicsManager::Render(StrongGameActorPtrList gameActors, StrongGameActorPtr currentCamera, SDL_Renderer* renderer)
{
	StrongActorComponentPtr cameraTransformComponent = currentCamera->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawCameraTransformComponent = std::dynamic_pointer_cast<TransformComponent>(cameraTransformComponent);
	auto cameraPos = rawCameraTransformComponent->GetPosition();

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

		std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actorTransformComponent);
		auto actorPos = rawActorTransformComponent->GetPosition();
		auto actorSize = rawActorTransformComponent->GetSize();

		std::shared_ptr<GraphicsComponent> rawGraphicsComponent = std::dynamic_pointer_cast<GraphicsComponent>(graphicsComponent);

		SDL_Rect imgPartRect;
		imgPartRect.x = actorPos.x - cameraPos.x - rawGraphicsComponent->imageOffset.x;
		imgPartRect.y = actorPos.y - cameraPos.y - rawGraphicsComponent->imageOffset.y;
		imgPartRect.w = actorSize.x;
		imgPartRect.h = actorSize.y;

		SDL_RenderCopy(renderer, rawGraphicsComponent->m_sprite, &(rawGraphicsComponent->animationFrameRect), &imgPartRect);
	}
}

void GraphicsManager::RenderBackground(SDL_Texture* sprite, StrongGameActorPtr currentCamera, SDL_Renderer* renderer, int screenWidth, int screenHeight)
{
	StrongActorComponentPtr cameraTransformComponent = currentCamera->GetComponentByName(EComponentNames::TransformComponentEnum);
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	std::shared_ptr<TransformComponent> rawCameraTransformComponent = std::dynamic_pointer_cast<TransformComponent>(cameraTransformComponent);
	auto cameraPos = rawCameraTransformComponent->GetPosition();

	int spriteWidth, spriteHeight;
	SDL_QueryTexture(sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	for (int i = 0; i < screenWidth; i += spriteWidth)
	{
		for (int j = 0; j < screenHeight; j += spriteHeight)
		{
			SDL_Rect imgPartRect;
			imgPartRect.x = i - cameraPos.x;
			imgPartRect.y = j - cameraPos.y;
			imgPartRect.w = spriteWidth;
			imgPartRect.h = spriteHeight;

			SDL_RenderCopy(renderer, sprite, NULL, &imgPartRect);
		}
	}
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

		std::shared_ptr<TransformComponent> rawActorTransformComponent = std::dynamic_pointer_cast<TransformComponent>(actorTransformComponent);
		auto actorPos = rawActorTransformComponent->GetPosition();
		auto actorSize = rawActorTransformComponent->GetSize();

		std::shared_ptr<GraphicsComponent> rawGraphicsComponent = std::dynamic_pointer_cast<GraphicsComponent>(graphicsComponent);

		SDL_Rect imgPartRect;
		imgPartRect.x = actorPos.x - cameraPos.x - rawGraphicsComponent->imageOffset.x;
		imgPartRect.y = actorPos.y - cameraPos.y - rawGraphicsComponent->imageOffset.y;
		imgPartRect.w = actorSize.x;
		imgPartRect.h = actorSize.y;


		float x = actorPos.x - cameraPos.x - rawGraphicsComponent->imageOffset.x;
		float y = actorPos.y - cameraPos.y - rawGraphicsComponent->imageOffset.y;
		float w = actorSize.x;
		float h = actorSize.y;

		// Texture code
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char *data = stbi_load("resources/SpriteSheet.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		// ..:: Triangle Drawing code (in Game loop) :: ..
		/*glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);*/

		// Draw triangles with texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//SDL_RenderCopy(renderer, rawGraphicsComponent->m_sprite, &(rawGraphicsComponent->animationFrameRect), &imgPartRect);
	}
}

void GraphicsManager::ClearScreen()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(m_window);
}