#pragma once
#include "ActorComponent.h"
#include "Vector2D.h"
#include "Texture2D.h"

#include <string>
#include <memory>
#include <glew.h>

class GameActor;
class TransformComponent;
struct SDL_Texture;

class GraphicsComponent :
    public ActorComponent
{
public:
    GraphicsComponent(ComponentId componentId, 
                      const std::string& texturePath, 
                      int animationTimer, 
                      const std::shared_ptr<TransformComponent>& transformCompPtr);
    virtual ~GraphicsComponent();

    virtual void Update(GameActor& actor, float deltaMs) override;
    virtual void Update();

    virtual const EComponentNames GetComponentName() const override;

    std::shared_ptr<Texture2D> GetTexture() const { return m_texturePtr; };
    void SetTexture(std::string texturePath) { m_texturePtr = std::make_shared<Texture2D>(texturePath); };
    std::shared_ptr<TransformComponent> GetTransformCompPtr() const { return m_transformCompPtr; };
    Vector2D<float> GetImageOffset() const { return m_imageOffset; };
    Vector2D<GLfloat> GetTexturePos() const;
    Vector2D<GLfloat> GetTextureSize() const;

private:
    std::shared_ptr<Texture2D> m_texturePtr;
    std::shared_ptr<TransformComponent> m_transformCompPtr;
    Vector2D<float> m_imageOffset;
    int m_curAnimationTime, m_animationTimer;
    Vector2D<GLfloat> m_texturePos;
};

