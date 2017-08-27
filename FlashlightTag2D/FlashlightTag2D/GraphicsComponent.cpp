#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

GraphicsComponent::GraphicsComponent(SDL_Texture* sprite, int animationTimer, std::shared_ptr<TransformComponent> transformComponent)
	: m_sprite(sprite),
	  m_animationTimer(animationTimer),
	  m_transformComponent(transformComponent),
	  m_imageOffset(transformComponent->GetSize() / 2)
{
	m_xFrame = 0;
	m_yFrame = 0;
	m_curAnimationTime = animationTimer;
	m_animationFrameRect.x = 0; m_animationFrameRect.y = 0;
	m_animationFrameRect.h = 0; m_animationFrameRect.w = 0;
}

GraphicsComponent::GraphicsComponent(int animationTimer, std::shared_ptr<TransformComponent> transformComponent)
	: m_animationTimer(animationTimer),
	  m_transformComponent(transformComponent),
	  m_imageOffset(transformComponent->GetSize() / 2)
{
	m_xFrame = 0;
	m_yFrame = 0;
	m_curAnimationTime = animationTimer;
	m_animationFrameRect.x = 0; m_animationFrameRect.y = 0;
	m_animationFrameRect.h = 0; m_animationFrameRect.w = 0;
}

GraphicsComponent::~GraphicsComponent()
{
	SDL_DestroyTexture(m_sprite);
	m_sprite = NULL;
}

ComponentId GraphicsComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames GraphicsComponent::GetComponentName() const
{
	return EComponentNames::GraphicsComponentEnum;
}

std::vector<float> GraphicsComponent::GetScaledVertices(int screenWidth, int screenHeight)
{
	std::vector<float> vertices;

	if (screenHeight < 1 || screenWidth < 1)
	{
		return vertices;
	}

	auto pos = m_transformComponent->GetPosition();

	// Top Right
	vertices.push_back((pos.x + m_imageOffset.x)/screenWidth); vertices.push_back((pos.y - m_imageOffset.y) / screenHeight); vertices.push_back(0);
	//vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
	// Bottom Right
	vertices.push_back((pos.x + m_imageOffset.x) / screenWidth); vertices.push_back((pos.y + m_imageOffset.y) / screenHeight); vertices.push_back(0);
	//vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
	// Bottom Left
	vertices.push_back((pos.x - m_imageOffset.x) / screenWidth); vertices.push_back((pos.y + m_imageOffset.y) / screenHeight); vertices.push_back(0);
	//vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);
	// Top Left 
	vertices.push_back((pos.x - m_imageOffset.x) / screenWidth); vertices.push_back((pos.y - m_imageOffset.y) / screenHeight); vertices.push_back(0);
	//vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);

	return vertices;
}

void GraphicsComponent::Update(GameActor& actor, int deltaMs)
{
	/*int spriteWidth, spriteHeight;
	SDL_QueryTexture(m_sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	animationFrameRect.x = xFrame;
	animationFrameRect.y = yFrame;
	animationFrameRect.w = xFrame + m_TransformComponent->GetSize().x;
	animationFrameRect.h = yFrame + m_TransformComponent->GetSize().y;

	if (curAnimationTime < 0)
	{
		curAnimationTime = m_animationTimer;

		xFrame = (int)(xFrame + m_TransformComponent->GetSize().x) % spriteWidth;

		if (xFrame == 0)
			yFrame = (int)(yFrame + m_TransformComponent->GetSize().y) % spriteHeight;
	}
	else
	{
		curAnimationTime--;
	}*/
}