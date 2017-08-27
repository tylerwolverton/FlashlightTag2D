#pragma once
#include "ActorComponent.h"
#include "SDLUtils.h"
#include "Vector2D.h"

#include <vector>

class GameActor;
class TransformComponent;
struct SDL_Texture;

class GraphicsComponent :
	public ActorComponent
{
public:
	SDL_Rect m_animationFrameRect;
	SDL_Texture* m_sprite;
	std::shared_ptr<TransformComponent> m_transformComponent;
	Vector2D<float> m_imageOffset;

	GraphicsComponent(SDL_Texture* sprite, int animationTimer, std::shared_ptr<TransformComponent> transformComponent);
	GraphicsComponent(int animationTimer, std::shared_ptr<TransformComponent> transformComponent);
	virtual ~GraphicsComponent();

	virtual void Update(GameActor& actor, int deltaMs) override;

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

	std::vector<float> GetScaledVertices(int screenWidth, int screenHeight);

private:
	int m_curAnimationTime, m_animationTimer;
	int m_xFrame, m_yFrame;
};

