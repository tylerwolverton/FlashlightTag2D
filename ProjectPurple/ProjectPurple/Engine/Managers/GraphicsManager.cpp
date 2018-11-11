#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "GameActor.h"
#include "Vector2D.h"
#include "Level.h"

GraphicsManager::GraphicsManager(SDL_Window* windowPtr)
    : m_windowPtr(windowPtr),
      m_lastComponentId(0)
{
    setOpenGLAttributes();

    // Create our opengl context and attach it to our window
    m_mainContext = SDL_GL_CreateContext(m_windowPtr);

    // This makes our buffer swap synchronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    initializeRenderData();

    initializeTiles();
}

GraphicsManager::~GraphicsManager()
{
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_quadVAO);

    // Delete our OpengL context
    SDL_GL_DeleteContext(m_mainContext);
}

void GraphicsManager::Reset()
{
    m_graphicsCompPtrMap.clear();
    m_cameraPtrVec.clear();
    m_curCameraPtr = nullptr;
    m_backgroundTexturePtr = nullptr;

    for (auto vec : m_backgroundTilePtrVecVec)
    {
        vec.clear();
    }
    m_backgroundTilePtrVecVec.clear();
}

void GraphicsManager::LoadNewLevel(std::shared_ptr<Level> level)
{
    m_curLevelPtr = level;

    if (m_curLevelPtr->GetSpritePath() != "")
    {
        m_backgroundTexturePtr = std::make_shared<Texture2D>(m_curLevelPtr->GetSpritePath());
    }
    
    int textureVecIdx = 0;
    auto tileVec = m_curLevelPtr->GetTileVec();
    for (int i = tileVec.size() - 1; i >= 0; i--)
    {
        m_backgroundTilePtrVecVec.push_back(std::vector<std::shared_ptr<Texture2D>>());
        for (auto tile : tileVec[i])
        {
            m_backgroundTilePtrVecVec[textureVecIdx].push_back(m_tileTextureVec[tile->GetSpriteIdx()]);
        }
        textureVecIdx++;
    }

    std::shared_ptr<Shader> shaderPtr = m_curLevelPtr->GetShader();
    if (shaderPtr->Init())
    {
        shaderPtr->UseProgram();
    }

    int windowWidth, windowHeight;
    SDL_GetWindowSize(m_windowPtr, &windowWidth, &windowHeight);
    m_projMatrixPtr = Matrix4<GLfloat>::CreateOrthoMatrix(0, (float)windowWidth, (float)windowHeight, 0, -1, 1);
    shaderPtr->SetMatrix4("projection", m_projMatrixPtr->GetPtrToFlattenedData().get());
    shaderPtr->SetInt("image", 0);
}

void GraphicsManager::AddGraphicsComponentPtr(ComponentId compId, std::shared_ptr<GraphicsComponent> comp)
{
    m_graphicsCompPtrMap.insert(std::make_pair(compId, comp));
}

void GraphicsManager::RemoveGraphicsComponentPtr(ComponentId compId)
{
    m_graphicsCompPtrMap.erase(compId);
}

// TODO: Cache changes
//void GraphicsManager::AddGraphicsComponent(GraphicsComponent comp) 
//{ 
//    m_graphicsComponentVec.push_back(comp); 
//};
//
//int GraphicsManager::AddGraphicsComponent(std::string texturePath, int animationTimer, StrongTransformComponentPtr transformComponent)
//{
//	int compId = getNextComponentId();
//	m_graphicsComponentVec.emplace_back(compId, texturePath, animationTimer, transformComponent);
//	return compId;
//}

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

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void GraphicsManager::initializeTiles()
{
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Art/Tiles/Tile00.png"));
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Art/Tiles/Tile01.png"));
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Art/Tiles/Tile02.png"));
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Art/Tiles/Wall01.png"));
}

//void GraphicsManager::UpdateComponents()
//{
//	for (auto& comp : m_graphicsComponentVec)
//	{
//		comp->Update();
//	}
//}

void GraphicsManager::AddCamera(std::shared_ptr<GameActor> camera)
{
    if (m_curCameraPtr == nullptr)
    {
        m_curCameraPtr = camera;
    }

    m_cameraPtrVec.push_back(camera);
}

void GraphicsManager::Render()
{
    auto cameraTransformCompPtr = m_curCameraPtr->GetTransformCompPtr();
    if (cameraTransformCompPtr == nullptr) 
	{ 
		return;
	}

    Vector2D<float> cameraPos = cameraTransformCompPtr->GetPosition();
    Vector2D<float> cameraSize = cameraTransformCompPtr->GetSize();

    glClear(GL_COLOR_BUFFER_BIT);

    std::shared_ptr<Shader> shaderPtr = m_curLevelPtr->GetShader();
    
    renderBackground(cameraPos);

    m_curLevelPtr->PrepShaders(m_graphicsCompPtrMap, cameraPos);

    for (auto graphicsCompEntry : m_graphicsCompPtrMap)
    {
        auto actorTransformCompPtr = *(graphicsCompEntry.second->GetTransformCompPtr());

        Vector2D<float> actorPos = actorTransformCompPtr.GetPosition();
        Vector2D<float> actorSize = actorTransformCompPtr.GetSize();

        if (actorPos.x + actorSize.x < cameraPos.x || actorPos.x - actorSize.x > cameraPos.x + cameraSize.x
            || actorPos.y + actorSize.y < cameraPos.y || actorPos.y - actorSize.y > cameraPos.y + cameraSize.y)
        {
            continue;
        }

        // Prepare transformations
        Matrix4<GLfloat> model;
        Vector2D<float> actorLocation = actorPos - cameraPos - graphicsCompEntry.second->GetImageOffset();
        model = model.Translate(actorLocation);

        // TODO: find a better scaling method
        model = model.Scale(actorSize);

        shaderPtr->SetMatrix4("model", model.GetPtrToFlattenedData().get());
        shaderPtr->SetVec2("textureSize", graphicsCompEntry.second->GetTextureSize().GetPtrToFlattenedData().get());
        shaderPtr->SetVec2("texturePos", graphicsCompEntry.second->GetTexturePos().GetPtrToFlattenedData().get());
        
        glActiveTexture(GL_TEXTURE0);
                
		graphicsCompEntry.second->GetTexture()->BindTexture();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glBindVertexArray(this->m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    SDL_GL_SwapWindow(m_windowPtr);
}

void GraphicsManager::renderBackground(Vector2D<float> cameraPos)
{
    // Render the background
    if (m_backgroundTexturePtr != nullptr && m_backgroundTexturePtr->GetWidth() != 0)
    {
        std::shared_ptr<Shader> shaderPtr = m_curLevelPtr->GetShader();

        // Use the whole texture starting from the top left
        Vector2D<GLfloat> textureSize(1.0f, 1.0f);
        Vector2D<GLfloat> texturePos(0, 0);

        Matrix4<GLfloat> backgroundModel;
        backgroundModel = backgroundModel.Translate(-cameraPos);
        
        // Scale by the actual size of the sprite
        Vector2D<GLfloat> spriteSize((float)m_backgroundTexturePtr->GetWidth(), (float)m_backgroundTexturePtr->GetHeight());
        backgroundModel = backgroundModel.Scale(spriteSize);

        shaderPtr->SetMatrix4("model", backgroundModel.GetPtrToFlattenedData().get());
        shaderPtr->SetVec2("textureSize", textureSize.GetPtrToFlattenedData().get());
        shaderPtr->SetVec2("texturePos", texturePos.GetPtrToFlattenedData().get());
        
        glActiveTexture(GL_TEXTURE0);

        m_backgroundTexturePtr->BindTexture();

        glBindVertexArray(this->m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    else
    {
        std::shared_ptr<Shader> shaderPtr = m_curLevelPtr->GetShader();

        // Use the whole texture starting from the top left
        Vector2D<GLfloat> textureSize(1.0f, 1.0f);
        Vector2D<GLfloat> texturePos(0, 0);

        shaderPtr->SetVec2("textureSize", textureSize.GetPtrToFlattenedData().get());
        shaderPtr->SetVec2("texturePos", texturePos.GetPtrToFlattenedData().get());

        int textureVecIdx = 0;
        for (auto gameTilePtrVec : m_curLevelPtr->GetTileVec())
        {
            int texX = 0;
            for (auto tilePtr : gameTilePtrVec)
            {
                Matrix4<GLfloat> backgroundModel;
                auto position = tilePtr->GetTransformCompPtr()->GetPosition() - cameraPos + Vector2D<float>(0, -Level::TILE_HEIGHT);
                backgroundModel = backgroundModel.Translate(position);

                // Scale by the actual size of the sprite
                auto tileTexPtr = m_tileTextureVec[tilePtr->GetSpriteIdx()];
                Vector2D<GLfloat> spriteSize((float)tileTexPtr->GetWidth(), (float)tileTexPtr->GetHeight());
                backgroundModel = backgroundModel.Scale(spriteSize);

                shaderPtr->SetMatrix4("model", backgroundModel.GetPtrToFlattenedData().get());

                glActiveTexture(GL_TEXTURE0);

				tileTexPtr->BindTexture();

                glBindVertexArray(this->m_quadVAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                texX++;
            }
            textureVecIdx++;
        }
    }
}