#pragma once
#include "ActorComponent.h"
class PhysicsComponent :
	public ActorComponent
{
public:
	PhysicsComponent();
	virtual ~PhysicsComponent();

	virtual bool Init(tinyxml2::XMLElement* pData) override;
	virtual void PostInit() override;
	virtual void Update(GameActor& actor, int deltaMs) override;

	virtual ComponentId GetComponentId() const override;
	virtual EComponentNames GetComponentName() const override;
};

