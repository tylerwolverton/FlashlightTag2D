#pragma once
#include <glew.h>
#include <memory>
#include <map>
#include <string>
#include "Types.h"
#include "Matrix4.h"
#include "Shader.h"
#include "Texture2D.h"

#define GL3_PROTOTYPES 1

class GraphicsComponent;
class GameActor;
class Level;

struct SDL_Window;

class GraphicsManager
{
public:
    GraphicsManager(SDL_Window* window);
    ~GraphicsManager();

    void Reset();

    void Render();
    void ClearScreen();

    void LoadNewLevel(std::shared_ptr<Level> level);
    
    void AddGraphicsComponentPtr(ComponentId compId, std::shared_ptr<GraphicsComponent> comp);
    void RemoveGraphicsComponentPtr(ComponentId compId);

    // TODO: Cache changes
    //void AddGraphicsComponent(GraphicsComponent comp);
    //int AddGraphicsComponent(std::string texturePath, int animationTimer, StrongTransformComponentPtr transformComponent);

    //void RemoveGraphicsComponent();
    //void UpdateComponents();

    void AddCamera(std::shared_ptr<GameActor> camera);
    std::shared_ptr<GameActor> GetCurrentCamera() { return m_curCameraPtr; }

private:
    SDL_Window* m_windowPtr;

    // Our opengl context handle
    SDL_GLContext m_mainContext;

    std::shared_ptr<Level> m_curLevelPtr;
    GLuint m_VBO, m_quadVAO;
    
    std::unique_ptr<Matrix4<GLfloat>> m_projMatrixPtr;

    std::shared_ptr<Texture2D> m_backgroundTexturePtr;
    std::vector<std::vector<std::shared_ptr<Texture2D>>> m_backgroundTilePtrVecVec;
	    
    // TODO: Cache changes
    //std::vector<GraphicsComponent> m_graphicsComponentVec;
    //std::vector<std::shared_ptr<GraphicsComponent>> m_graphicsComponentPtrVec;
    std::map<ComponentId, std::shared_ptr<GraphicsComponent>> m_graphicsCompPtrMap;

    std::vector<std::shared_ptr<GameActor>> m_cameraPtrVec;
    std::shared_ptr<GameActor> m_curCameraPtr;

    int m_lastComponentId;
    int getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };

    std::vector<std::shared_ptr<Texture2D>> m_tileTextureVec;

	bool setOpenGLAttributes();
	bool initializeRenderData();
	void initializeTiles();
	void renderBackground(Vector2D<float> cameraPos);
};

