#pragma once
#include "ActorComponent.h"
#include "SDLUtils.h"
#include "Vector2D.h"
#include "Texture2D.h"

#include <vector>

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
	SDL_Rect GetAnimationFrameRect() { return m_animationFrameRect; };
	Texture2D GetTexture() { return m_texture; };
	std::shared_ptr<TransformComponent> GetTransformComponent() { return m_pTransformComponent; };
	Vector2D<float> GetImageOffset() { return m_imageOffset; };

private:
	SDL_Rect m_animationFrameRect;
	Texture2D m_texture;
	std::shared_ptr<TransformComponent> m_pTransformComponent;
	Vector2D<float> m_imageOffset;
	int m_curAnimationTime, m_animationTimer;
	int m_xFrame, m_yFrame;
};

