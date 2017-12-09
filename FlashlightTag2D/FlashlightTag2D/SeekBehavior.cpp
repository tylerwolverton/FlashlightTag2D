#include "SeekBehavior.h"
#include "ActorFactory.h"
#include "ServiceLocator.h"
#include "Vector2D.h"

SeekBehavior::SeekBehavior()
{
	m_currentTarget = nullptr;
}

SeekBehavior::~SeekBehavior()
{
}

CommandList SeekBehavior::Update()
{
	// Use the current light cone and sound in surrounding area to see if any actors are close, then if a hider is seen, enter the chase state
	// If already chasing and the hider is lost for long enough, enter the seek state (create a ChaseBahavior?)

	// if(state == find)
	// {
		// Get list of nearby actors
		auto actorList = ServiceLocator::GetActorFactory()->GetActorList();
		//StrongGameActorPtrList actorsInRange;
		// Find closest hider in that list
		// Implicitly set max range, variable-ize this later
		auto distToClosestActor = new Vector2D<float>(300.0f, 300.0f);
		for (auto actor : actorList)
		{
			// TODO: Rethink this type naming system to find the right game actors
			// Create game state component? Contains actor type, enum for seeker or hider, etc.
			auto actorName = actor->GetActorClassName();
			if ((actorName == "Enemy" || actorName == "Player"))
			//	&& actor->GetComponentByName(EComponentNames::TransformComponentEnum)... - thisActorPosition .Length())
			{

			}
		}
		
		// if(foundHider)
		// {
		//		currentTarget = hider
	    //      state = chase
		// } 
		// else
		// {
		//		return MoveInSearchPatern()
		// }
	// }
	// else if(state == chase)
	// {
	    // if (closeEnough)
		// {
	    //      tagHider()
		// } 
		// else if (notTooFarAway)
		// {
		//		return MoveTowardsActor()
		// }
		//
		//  If we tagged the target or they are too far away, return to find state
		//	currentTarget = nullptr
		//  state = find
	// }

	return CommandList();
}

