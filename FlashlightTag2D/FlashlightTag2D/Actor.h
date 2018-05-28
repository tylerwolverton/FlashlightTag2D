#pragma once
#include "Types.h"

#include <map>
#include <memory>

class ActorComponent;

class Actor
{
	friend class ActorFactory;

public:
	typedef std::map<ComponentId, std::shared_ptr<ActorComponent>> ActorComponentMap;

public:
	Actor(ActorId id);
	virtual ~Actor();

	bool Init();
	void PostInit();
	void Destroy();
	virtual void Update(int delatMs);

	ActorId GetId() const { return m_id; }

private:
	void add_component(std::shared_ptr<ActorComponent> pComponent);

	ActorId m_id;
	ActorComponentMap m_componentMap;
};

