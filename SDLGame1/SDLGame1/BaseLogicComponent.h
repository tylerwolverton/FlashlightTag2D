#pragma once
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "Behavior.h"

class BaseLogicComponent :
	public ActorComponent
{
public:
	BaseLogicComponent(std::shared_ptr<TransformComponent> transformComponent);
	virtual ~BaseLogicComponent();

	virtual void MoveUp()    { m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y - 10)); };
	virtual void MoveDown()  { m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y + 10)); };
	virtual void MoveRight() { m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x + 10, m_TransformComponent->GetPosition().y)); };
	virtual void MoveLeft()  { m_TransformComponent->SetPosition(Vector2D<float>(m_TransformComponent->GetPosition().x - 10, m_TransformComponent->GetPosition().y)); };

	virtual bool Init(tinyxml2::XMLElement* pData);
	virtual void PostInit() {}
	virtual void Update(GameActor& actor, int deltaMs) {}

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;

private:
	std::shared_ptr<TransformComponent> m_TransformComponent;
	std::shared_ptr<Behavior> behavior;
};

