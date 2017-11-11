#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

GraphicsComponent::GraphicsComponent(std::string texturePath, int animationTimer, std::shared_ptr<TransformComponent> transformComponent)
	: m_animationTimer(animationTimer),
	  m_curAnimationTime(animationTimer),
	  m_pTransformComponent(transformComponent),
	  m_imageOffset(transformComponent->GetSize() / 2),
	  m_texture(Texture2D(texturePath)),
	  m_texturePos(Vector2D<GLfloat>(0, 0))
{
	m_texturePos.y = m_texture.GetHeight() - transformComponent->GetSize().y;
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
	if (m_curAnimationTime < 0)
	{
		m_curAnimationTime = m_animationTimer;

		m_texturePos.x = (int)(m_texturePos.x + m_pTransformComponent->GetSize().x) % m_texture.GetWidth();
		
		if (m_texturePos.x == 0)
		{
			m_texturePos.y = (int)(m_texturePos.y - m_pTransformComponent->GetSize().y) % m_texture.GetHeight();
		}
	}
	else
	{
		m_curAnimationTime--;
	}
}

Vector2D<GLfloat> GraphicsComponent::GetTextureSize()
{
	auto size = Vector2D<GLfloat>(m_pTransformComponent->GetSize()); 
	size.x /= m_texture.GetWidth();
	size.y /= m_texture.GetHeight();

	return size;
}

Vector2D<GLfloat> GraphicsComponent::GetTexturePos()
{
	auto pos = m_texturePos;
	pos.x /= m_texture.GetWidth();
	pos.y /= m_texture.GetHeight();

	return pos;
}