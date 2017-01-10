#pragma once
#include "ActorFactory.h"

class ActorComponent
{
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent();

	virtual bool VInit() = 0;
	virtual void VPostInit() {}
	virtual void VUpdate(int deltaMs) {}

	virtual ComponentId VGetComponentId() const = 0;

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

