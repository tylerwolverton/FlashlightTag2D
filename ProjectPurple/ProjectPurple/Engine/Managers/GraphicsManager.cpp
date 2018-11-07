#include "GraphicsManager.h"
#include "ActorComponent.h"
#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameStateComponent.h"
#include "GameActor.h"
#include "Vector2D.h"
#include "Level.h"

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
    
    initializeRenderData();

    initializeTiles();
}

GraphicsManager::~GraphicsManager()
{
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &m_quadVAO);

    // Delete our OpengL context
    SDL_GL_DeleteContext(m_mainContext);
}

void GraphicsManager::Reset()
{
    m_graphicsComponentPtrMap.clear();
    m_pCameraVec.clear();
    m_pCurrentCamera = nullptr;
    m_backgroundTexture = nullptr;

    for (auto vec : m_backgroundTileVec)
    {
        vec.clear();
    }
    m_backgroundTileVec.clear();
}

void GraphicsManager::LoadNewLevel(std::shared_ptr<Level> level)
{
    m_curLevel = level;

    if (m_curLevel->GetSpritePath() != "")
    {
        m_backgroundTexture = std::make_shared<Texture2D>(m_curLevel->GetSpritePath());
    }
    
    int textureVecIdx = 0;
    auto tileVec = m_curLevel->GetTileVec();
    for (int i = tileVec.size() - 1; i >= 0; i--)
    {
        m_backgroundTileVec.push_back(std::vector<std::shared_ptr<Texture2D>>());
        for (auto tile : tileVec[i])
        {
            m_backgroundTileVec[textureVecIdx].push_back(m_tileTextureVec[tile->GetSpriteIdx()]);
        }
        textureVecIdx++;
    }

    std::shared_ptr<Shader> shader = m_curLevel->GetShader();
    if (shader->Init())
    {
        shader->UseProgram();
    }

    int windowWidth, windowHeight;
    SDL_GetWindowSize(m_window, &windowWidth, &windowHeight);
    m_projMatrix = Matrix4<GLfloat>::CreateOrthoMatrix(0, (float)windowWidth, (float)windowHeight, 0, -1, 1);
    shader->SetMatrix4("projection", m_projMatrix->GetPtrToFlattenedData().get());
    shader->SetInt("image", 0);
}

void GraphicsManager::AddGraphicsComponentPtr(ComponentId compId, std::shared_ptr<GraphicsComponent> comp)
{
    m_graphicsComponentPtrMap.insert(std::make_pair(compId, comp));
}

void GraphicsManager::RemoveGraphicsComponentPtr(ComponentId compId)
{
    m_graphicsComponentPtrMap.erase(compId);
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
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Tiles/Tile00.png"));
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Tiles/Tile01.png"));
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Tiles/Tile02.png"));
    m_tileTextureVec.push_back(std::make_shared<Texture2D>("resources/Tiles/Wall01.png"));
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
    if (m_pCurrentCamera == nullptr)
    {
        m_pCurrentCamera = camera;
    }

    m_pCameraVec.push_back(camera);
}

void GraphicsManager::Render()
{
    auto cameraTransformComponent = m_pCurrentCamera->GetTransformComponent();
    if (cameraTransformComponent == nullptr) { return; }

    Vector2D<float> cameraPos = cameraTransformComponent->GetPosition();
    Vector2D<float> cameraSize = cameraTransformComponent->GetSize();

    glClear(GL_COLOR_BUFFER_BIT);

    std::shared_ptr<Shader> shader = m_curLevel->GetShader();
    
    renderBackground(cameraPos);

    m_curLevel->PrepShaders(m_graphicsComponentPtrMap, cameraPos);


    for (auto graphicsComponent : m_graphicsComponentPtrMap)
    {
        auto actorTransformComponent = *(graphicsComponent.second->GetTransformComponent());   

        Vector2D<float> actorPos = actorTransformComponent.GetPosition();
        Vector2D<float> actorSize = actorTransformComponent.GetSize();

        if (actorPos.x < cameraPos.x || actorPos.x > cameraPos.x + cameraSize.x
            || actorPos.y < cameraPos.y || actorPos.y > cameraPos.y + cameraSize.y)
        {
            continue;
        }

        // Prepare transformations
        Matrix4<GLfloat> model;
        Vector2D<float> actorLocation = actorPos - cameraPos - graphicsComponent.second->GetImageOffset();
        model = model.Translate(actorLocation);

        // TODO: find a better scaling method
        model = model.Scale(actorSize);

        shader->SetMatrix4("model", model.GetPtrToFlattenedData().get());
        shader->SetVec2("textureSize", graphicsComponent.second->GetTextureSize().GetPtrToFlattenedData().get());
        shader->SetVec2("texturePos", graphicsComponent.second->GetTexturePos().GetPtrToFlattenedData().get());
        
        glActiveTexture(GL_TEXTURE0);
                
        graphicsComponent.second->GetTexture()->BindTexture();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glBindVertexArray(this->m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    SDL_GL_SwapWindow(m_window);
}

void GraphicsManager::renderBackground(Vector2D<float> cameraPos)
{
    // Render the background
    if (m_backgroundTexture != nullptr && m_backgroundTexture->GetWidth() != 0)
    {
        std::shared_ptr<Shader> shader = m_curLevel->GetShader();

        // Use the whole texture starting from the top left
        Vector2D<GLfloat> textureSize(1.0f, 1.0f);
        Vector2D<GLfloat> texturePos(0, 0);

        Matrix4<GLfloat> backgroundModel;
        backgroundModel = backgroundModel.Translate(-cameraPos);
        
        // Scale by the actual size of the sprite
        Vector2D<GLfloat> spriteSize((float)m_backgroundTexture->GetWidth(), (float)m_backgroundTexture->GetHeight());
        backgroundModel = backgroundModel.Scale(spriteSize);

        shader->SetMatrix4("model", backgroundModel.GetPtrToFlattenedData().get());
        shader->SetVec2("textureSize", textureSize.GetPtrToFlattenedData().get());
        shader->SetVec2("texturePos", texturePos.GetPtrToFlattenedData().get());
        
        glActiveTexture(GL_TEXTURE0);

        m_backgroundTexture->BindTexture();

        glBindVertexArray(this->m_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    else
    {
        std::shared_ptr<Shader> shader = m_curLevel->GetShader();

        // Use the whole texture starting from the top left
        Vector2D<GLfloat> textureSize(1.0f, 1.0f);
        Vector2D<GLfloat> texturePos(0, 0);

        shader->SetVec2("textureSize", textureSize.GetPtrToFlattenedData().get());
        shader->SetVec2("texturePos", texturePos.GetPtrToFlattenedData().get());

        int textureVecIdx = 0;
        for (auto vec : m_curLevel->GetTileVec())
        {
            int texX = 0;
            for (auto tile : vec)
            {
                //std::shared_ptr<Shader> shader = m_curLevel->GetShader();

                // Use the whole texture starting from the top left
                //Vector2D<GLfloat> textureSize(1.0f, 1.0f);
                //Vector2D<GLfloat> texturePos(0, 0);

                Matrix4<GLfloat> backgroundModel;
                //auto position = Vector2D<float>(tile->GetWidth() * texX, tile->GetHeight() * textureVecIdx) - cameraPos;
                auto position = tile->GetTransformComponent()->GetPosition() - cameraPos + Vector2D<float>(0, -Level::TILE_HEIGHT);
                backgroundModel = backgroundModel.Translate(position);

                // Scale by the actual size of the sprite
                auto tileTex = m_tileTextureVec[tile->GetSpriteIdx()];
                Vector2D<GLfloat> spriteSize((float)tileTex->GetWidth(), (float)tileTex->GetHeight());
                //Vector2D<GLfloat> spriteSize((float)tile->GetWidth(), (float)tile->GetHeight());
                backgroundModel = backgroundModel.Scale(spriteSize);

                shader->SetMatrix4("model", backgroundModel.GetPtrToFlattenedData().get());
                //shader->SetVec2("textureSize", textureSize.GetPtrToFlattenedData().get());
                //shader->SetVec2("texturePos", texturePos.GetPtrToFlattenedData().get());

                glActiveTexture(GL_TEXTURE0);

                //tile->BindTexture();
                tileTex->BindTexture();

                glBindVertexArray(this->m_quadVAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                texX++;
            }
            textureVecIdx++;
        }
    }
}

void GraphicsManager::ClearScreen()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(m_window);
}