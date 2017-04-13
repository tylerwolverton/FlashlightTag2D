#pragma once
#include "ActorComponent.h"
#include "SDLUtils.h"
#include "Vector2D.h"

class GameActor;
class TransformComponent;
struct SDL_Texture;

class GraphicsComponent :
	public ActorComponent
{
public:
	SDL_Rect animationFrameRect;
	SDL_Texture* m_sprite;
	std::shared_ptr<TransformComponent> m_TransformComponent;
	Vector2D<float> imageOffset;

	GraphicsComponent(SDL_Texture* sprite, int animationTimer, std::shared_ptr<TransformComponent> transformComponent);
	virtual ~GraphicsComponent();

	virtual void Update(GameActor& actor, int deltaMs) override;

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

private:
	int curAnimationTime, m_animationTimer;
	int xFrame, yFrame;
};

