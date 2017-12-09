#pragma once
#include "Behavior.h"
#include "Types.h"
class SeekBehavior :
	public Behavior
{
public:
	SeekBehavior();
	virtual ~SeekBehavior();

	virtual CommandList Update() override;

private:
	StrongGameActorPtr m_currentTarget;
};

