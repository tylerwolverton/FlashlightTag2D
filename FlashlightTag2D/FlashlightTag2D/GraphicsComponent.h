#pragma once
#include "ActorComponent.h"
#include "Types.h"
#include "Vector2D.h"

#include <glew.h>

class GameActor;
class TransformComponent;
struct SDL_Texture;

class GraphicsComponent :
	public ActorComponent
{
public:
	GraphicsComponent(ComponentId componentId, std::string texturePath, int animationTimer, StrongTransformComponentPtr transformComponent);
	virtual ~GraphicsComponent();

	virtual void Update(GameActor& actor, float deltaMs) override;

	virtual const EComponentNames GetComponentName() const override;

    StrongTexture2DPtr GetTexture() { return m_texture; };
    StrongTransformComponentPtr GetTransformComponent() { return m_pTransformComponent; };
	Vector2D<float> GetImageOffset() { return m_imageOffset; };
	Vector2D<GLfloat> GetTexturePos();
	Vector2D<GLfloat> GetTextureSize();

private:
    StrongTexture2DPtr m_texture;
	StrongTransformComponentPtr m_pTransformComponent;
	Vector2D<float> m_imageOffset;
	int m_curAnimationTime, m_animationTimer;
	Vector2D<GLfloat> m_texturePos;
};

