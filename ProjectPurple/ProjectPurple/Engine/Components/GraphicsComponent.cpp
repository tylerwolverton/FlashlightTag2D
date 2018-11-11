#include "GraphicsComponent.h"
#include "TransformComponent.h"
#include "GameActor.h"
#include "World.h"

GraphicsComponent::GraphicsComponent(ComponentId componentId, 
                                     std::string texturePath, 
                                     int animationTimer, 
                                     std::shared_ptr<TransformComponent> transformCompPtr)
    : ActorComponent(componentId),
      m_animationTimer(animationTimer),
      m_curAnimationTime(animationTimer),
      m_transformCompPtr(transformCompPtr),
      m_imageOffset(transformCompPtr->GetSize() / 2),
      m_texturePtr(std::make_shared<Texture2D>(texturePath)),
      m_texturePos(Vector2D<GLfloat>(0, 0))
{
    // Initialize to the first sprite of the sprite sheet
    m_texturePos.y = m_texturePtr->GetHeight() - transformCompPtr->GetSize().y;
}

GraphicsComponent::~GraphicsComponent()
{
}

const EComponentNames GraphicsComponent::GetComponentName() const
{
    return EComponentNames::GraphicsComponentEnum;
}

void GraphicsComponent::Update(GameActor& actor, float deltaMs)
{
    if (m_animationTimer > 0)
    {
        if (m_curAnimationTime < 0)
        {
            m_curAnimationTime = m_animationTimer;

            m_texturePos.x = (float)((int)(m_texturePos.x + m_transformCompPtr->GetSize().x) % m_texturePtr->GetWidth());

            // Move to next row of sprite sheet
            if (m_texturePos.x == 0)
            {
                m_texturePos.y = (float)((int)(m_texturePos.y - m_transformCompPtr->GetSize().y) % m_texturePtr->GetHeight());
            }
        }
        else
        {
            m_curAnimationTime--;
        }
    }
}

void GraphicsComponent::Update()
{
    if (m_curAnimationTime < 0)
    {
        m_curAnimationTime = m_animationTimer;

        m_texturePos.x = (float)((int)(m_texturePos.x + m_transformCompPtr->GetSize().x) % m_texturePtr->GetWidth());

        // Move to next row of sprite sheet
        if (m_texturePos.x == 0)
        {
            m_texturePos.y = (float)((int)(m_texturePos.y - m_transformCompPtr->GetSize().y) % m_texturePtr->GetHeight());
        }
    }
    else
    {
        m_curAnimationTime--;
    }
}

// Get the size of the texture and normalize to value between 0 and 1
Vector2D<GLfloat> GraphicsComponent::GetTextureSize()
{
    auto size = Vector2D<GLfloat>(m_transformCompPtr->GetSize()); 
    size.x /= m_texturePtr->GetWidth();
    size.y /= m_texturePtr->GetHeight();

    return size;
}

// Get the position of the texture in the spritesheet, normalized to between 0 and 1.
Vector2D<GLfloat> GraphicsComponent::GetTexturePos()
{
    auto pos = m_texturePos;
    pos.x /= m_texturePtr->GetWidth();
    pos.y /= m_texturePtr->GetHeight();

    return pos;
}