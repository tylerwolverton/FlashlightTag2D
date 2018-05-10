#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"

#include <string>
#include <memory>
#include <glew.h>

class GameActor;
class TransformComponent;
class Texture2D;
struct SDL_Texture;

class GraphicsComponent :
	public ActorComponent
{
public:
	GraphicsComponent(ComponentId componentId, std::string texturePath, int animationTimer, std::shared_ptr<TransformComponent> transformComponent);
	virtual ~GraphicsComponent();

	virtual void Update(GameActor& actor, float deltaMs) override;
	virtual void Update();

	virtual const EComponentNames GetComponentName() const override;

    std::shared_ptr<Texture2D> GetTexture() { return m_texture; };
    std::shared_ptr<TransformComponent> GetTransformComponent() { return m_pTransformComponent; };
	Vector2D<float> GetImageOffset() { return m_imageOffset; };
	Vector2D<GLfloat> GetTexturePos();
	Vector2D<GLfloat> GetTextureSize();

private:
    std::shared_ptr<Texture2D> m_texture;
	std::shared_ptr<TransformComponent> m_pTransformComponent;
	Vector2D<float> m_imageOffset;
	int m_curAnimationTime, m_animationTimer;
	Vector2D<GLfloat> m_texturePos;
};

