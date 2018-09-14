#pragma once
#include <glew.h>
#include <memory>
#include <map>
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
    //void SetBackgroundTexture(std::string texturePath) { m_backgroundTexture = std::make_shared<Texture2D>(texturePath); };
    
    void AddGraphicsComponentPtr(ComponentId compId, std::shared_ptr<GraphicsComponent> comp);
    void RemoveGraphicsComponentPtr(ComponentId compId);

    // TODO: Cache changes
    //void AddGraphicsComponent(GraphicsComponent comp);
    //int AddGraphicsComponent(std::string texturePath, int animationTimer, StrongTransformComponentPtr transformComponent);

    //void RemoveGraphicsComponent();
	//void UpdateComponents();

    void AddCamera(std::shared_ptr<GameActor> camera);
	std::shared_ptr<GameActor> GetCurrentCamera() { return m_pCurrentCamera; }

private:
	SDL_Window* m_window;

	// Our opengl context handle
	SDL_GLContext m_mainContext;

	std::shared_ptr<Level> m_curLevel;
	//std::shared_ptr<Shader> m_shader;
	GLuint VBO, m_quadVAO;
	
	std::unique_ptr<Matrix4<GLfloat>> m_projMatrix;

    std::shared_ptr<Texture2D> m_backgroundTexture;

	bool setOpenGLAttributes();
	bool initializeRenderData();
    void renderBackground(Vector2D<float> cameraPos);
    
    // TODO: Cache changes
    //std::vector<GraphicsComponent> m_graphicsComponentVec;
    //std::vector<std::shared_ptr<GraphicsComponent>> m_graphicsComponentPtrVec;
    std::map<ComponentId, std::shared_ptr<GraphicsComponent>> m_graphicsComponentPtrMap;

    std::vector<std::shared_ptr<GameActor>> m_pCameraVec;
    std::shared_ptr<GameActor> m_pCurrentCamera;

	int m_lastComponentId;
	int getNextComponentId() { ++m_lastComponentId; return m_lastComponentId; };
};

