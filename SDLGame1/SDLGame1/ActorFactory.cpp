#include "ActorFactory.h"
#include "Actor.h"

ActorFactory::ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource)
{
	//// Grab the root XML node
	//TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
	//if (!pRoot)
	//{
	//	//GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
	//	return StrongActorPtr();
	//}

	//// create the actor instance
	////ActorId nextActorId = serversActorId;
	////if (nextActorId == INVALID_ACTOR_ID)
	////{
	//	//nextActorId = GetNextActorId();
	////}

	//StrongActorPtr pActor(new Actor(GetNextActorId()));
	//if (!pActor->Init(pRoot))
	//{
	//	//GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
	//	return StrongActorPtr();
	//}

	//bool initialTransformSet = false;

	//// Loop through each child element and load the component
	//for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	//{
	//	StrongActorComponentPtr pComponent(VCreateComponent(pNode));
	//	if (pComponent)
	//	{
	//		pActor->AddComponent(pComponent);
	//		pComponent->SetOwner(pActor);
	//	}
	//	else
	//	{
	//		// If an error occurs, we kill the actor and bail.  We could keep going, but the actor is will only be 
	//		// partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
	//		// will fall out of scope with nothing else pointing to it.
	//		return StrongActorPtr();
	//	}
	//}

	//// Now that the actor has been fully created, run the post init phase
	//pActor->PostInit();

	StrongActorPtr pActor(new Actor(GetNextActorId()));

	return pActor;
}

StrongActorComponentPtr ActorFactory::ActorComponentCreator()
{
	//StrongActorComponentPtr pActor();// new ActorComponent(GetNextComponentId()));

	return StrongActorComponentPtr();
}