#pragma once
#include <map>
#include <memory>

#include "Types.h"

class Actor
{
	friend class ActorFactory;

public:
	typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponentMap;

private:
	ActorId m_id;
	ActorComponentMap m_componentMap;

public:
	explicit Actor(ActorId id);
	virtual ~Actor();

	bool Init();
	void PostInit();
	void Destroy();
	void Update(int delatMs);

	ActorId GetId() const { return m_id; }

	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(ComponentId id)
	{
		ActorComponentMap::iterator findIt = m_componentMap.find(id);

		if (findIt == m_componentMap.end())
		{
			return std::weak_ptr<ComponentType>();
		}

		StrongActorComponentPtr pBase = findIt->second;
		// cast to subclass version of pointer
		std::shared_ptr<ComponentType> pSub(std::tr1::static_pointer_cast<ComponentType>(pBase));
		std::weak_ptr<ComponentType> pWeakSub(pSub);

		return pWeakSub;
	}

private:
	void AddComponent(StrongActorComponentPtr pComponent);
};

