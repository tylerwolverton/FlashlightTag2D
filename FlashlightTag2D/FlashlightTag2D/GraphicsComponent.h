#pragma once
#include "ActorComponent.h"
#include "SDLUtils.h"
#include "Vector2D.h"
#include "Texture2D.h"

#include <vector>
#include <glew.h>

class GameActor;
class TransformComponent;
struct SDL_Texture;

class GraphicsComponent :
	public ActorComponent
{
public:
	GraphicsComponent(std::string texturePath, int animationTimer, std::shared_ptr<TransformComponent> transformComponent);
	virtual ~GraphicsComponent();

	virtual void Update(GameActor& actor, int deltaMs) override;

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

	//std::vector<float> GetScaledVertices(int screenWidth, int screenHeight);
	Texture2D GetTexture() { return m_texture; };
	std::shared_ptr<TransformComponent> GetTransformComponent() { return m_pTransformComponent; };
	Vector2D<float> GetImageOffset() { return m_imageOffset; };
	Vector2D<GLfloat> GetTexturePos();
	Vector2D<GLfloat> GetTextureSize();

private:
	Texture2D m_texture;
	std::shared_ptr<TransformComponent> m_pTransformComponent;
	Vector2D<float> m_imageOffset;
	int m_curAnimationTime, m_animationTimer;
	Vector2D<GLfloat> m_texturePos;
};

