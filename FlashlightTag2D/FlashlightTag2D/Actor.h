#pragma once
#include <map>
#include <memory>

#include "Types.h"

class Actor
{
	friend class ActorFactory;

public:
	typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponentMap;

public:
	Actor(ActorId id);
	virtual ~Actor();

	bool Init();
	void PostInit();
	void Destroy();
	virtual void Update(int delatMs);

	ActorId GetId() const { return m_id; }

private:
	void add_component(StrongActorComponentPtr pComponent);

	ActorId m_id;
	ActorComponentMap m_componentMap;
};

