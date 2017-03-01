#pragma once
#include "ActorComponent.h"
#include "SDLUtils.h"

class GameActor;

class GraphicsComponent :
	public ActorComponent
{
public:
	SDL_Rect animationFrameRect;

	GraphicsComponent(int animationTimer);
	virtual ~GraphicsComponent();

	virtual bool Init(tinyxml2::XMLElement* pData) override;
	virtual void PostInit() override;
	virtual void Update(GameActor& actor, int deltaMs) override;

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

private:
	int curAnimationTime, m_animationTimer;
	int xFrame, yFrame;
};

