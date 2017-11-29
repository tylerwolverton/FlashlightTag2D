#pragma once
#include "Behavior.h"
class SeekBehavior :
	public Behavior
{
public:
	SeekBehavior();
	virtual ~SeekBehavior();

	// This class needs to use the current light cone to see if any actors are close, then if a hider is seen, enter the chase state
	// If already chasing and the hider is lost for long enough, enter the seek state (create a ChaseBahavior?)
};

