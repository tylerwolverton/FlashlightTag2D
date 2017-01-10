#include "ActorFactory.h"

ActorFactory::ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource)
{
	// Grab xml root node
	//TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnXmlElement(actorResource);
}
