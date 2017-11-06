#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

GraphicsComponent::GraphicsComponent(std::string texturePath, int animationTimer, std::shared_ptr<TransformComponent> transformComponent)
	: m_animationTimer(animationTimer),
	  m_curAnimationTime(animationTimer),
	  m_pTransformComponent(transformComponent),
	  m_imageOffset(transformComponent->GetSize() / 2),
	  m_xFrame(0),
	  m_yFrame(0),
	  m_texture(Texture2D(texturePath))
{
	m_animationFrameRect.x = 0; m_animationFrameRect.y = 0;
	m_animationFrameRect.h = 0; m_animationFrameRect.w = 0;
}

GraphicsComponent::~GraphicsComponent()
{
}

ComponentId GraphicsComponent::GetComponentId() const
{
	return ComponentId();
}

EComponentNames GraphicsComponent::GetComponentName() const
{
	return EComponentNames::GraphicsComponentEnum;
}

//std::vector<float> GraphicsComponent::GetScaledVertices(int screenWidth, int screenHeight)
//{
//	std::vector<float> vertices;
//
//	if (screenHeight < 1 || screenWidth < 1)
//	{
//		return vertices;
//	}
//
//	auto pos = m_pTransformComponent->GetPosition();
//
//	// Top Right
//	vertices.push_back((pos.x + m_imageOffset.x)/screenWidth); vertices.push_back((pos.y - m_imageOffset.y) / screenHeight); vertices.push_back(0);
//	// Bottom Right
//	vertices.push_back((pos.x + m_imageOffset.x) / screenWidth); vertices.push_back((pos.y + m_imageOffset.y) / screenHeight); vertices.push_back(0);
//	// Bottom Left
//	vertices.push_back((pos.x - m_imageOffset.x) / screenWidth); vertices.push_back((pos.y + m_imageOffset.y) / screenHeight); vertices.push_back(0);
//	// Top Left 
//	vertices.push_back((pos.x - m_imageOffset.x) / screenWidth); vertices.push_back((pos.y - m_imageOffset.y) / screenHeight); vertices.push_back(0);
//
//	return vertices;
//}

void GraphicsComponent::Update(GameActor& actor, int deltaMs)
{
	/*int spriteWidth, spriteHeight;
	SDL_QueryTexture(m_sprite, NULL, NULL, &spriteWidth, &spriteHeight);

	m_animationFrameRect.x = m_xFrame;
	m_animationFrameRect.y = m_yFrame;
	m_animationFrameRect.w = m_xFrame + m_pTransformComponent->GetSize().x;
	m_animationFrameRect.h = m_yFrame + m_pTransformComponent->GetSize().y;

	if (m_curAnimationTime < 0)
	{
		m_curAnimationTime = m_animationTimer;

		m_xFrame = (int)(m_xFrame + m_pTransformComponent->GetSize().x) % spriteWidth;

		if (m_xFrame == 0)
			m_yFrame = (int)(m_yFrame + m_pTransformComponent->GetSize().y) % spriteHeight;
	}
	else
	{
		m_curAnimationTime--;
	}*/
}