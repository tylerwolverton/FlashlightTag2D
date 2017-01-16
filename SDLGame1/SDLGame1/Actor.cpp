#include "Actor.h"

Actor::Actor(ActorId id)
{
}


Actor::~Actor()
{
}

bool Actor::Init(tinyxml2::XMLElement* root)
{
	return true;
}

void Actor::PostInit() 
{

}

void Actor::Destroy()
{

}

void Actor::Update(int delatMs)
{

}

void Actor::AddComponent(StrongActorComponentPtr pComponent)
{

}