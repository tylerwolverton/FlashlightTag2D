#include "ActorFactory.h"
#include "Vector2D.h"
#include "Actor.h"
#include "GameActor.h"
#include "ActorComponent.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "AIComponent.h"
#include "FollowTargetAIComponent.h"
#include "World.h"

using namespace tinyxml2;

ActorFactory::ActorFactory(SDL_Renderer* renderer)
	: m_renderer(renderer)
{
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource)
{
	// Grab the root XML node
	XMLDocument doc;
	doc.LoadFile(actorResource);

	XMLElement* pRoot = doc.RootElement();//XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
	if (!pRoot)
	{
		//GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
		return StrongActorPtr();
	}

	// create the actor instance
	//ActorId nextActorId = serversActorId;
	//if (nextActorId == INVALID_ACTOR_ID)
	//{
	//	nextActorId = GetNextActorId();
	//}

	StrongActorPtr pActor(new Actor(GetNextActorId()));
	if (!pActor->Init(pRoot))
	{
		//GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
		return StrongActorPtr();
	}

	//bool initialTransformSet = false;

	// Loop through each child element and load the component
	for (XMLElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		StrongActorComponentPtr pComponent(VCreateComponent(pNode));
		if (pComponent)
		{
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			// If an error occurs, we kill the actor and bail.  We could keep going, but the actor is will only be 
			// partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
			// will fall out of scope with nothing else pointing to it.
			return StrongActorPtr();
		}
	}

	// Now that the actor has been fully created, run the post init phase
	pActor->PostInit();
	
	return pActor;
}

StrongGameActorPtr ActorFactory::CreatePlayer(StrongWorldPtr world)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<InputComponent>());
	auto sprite = loadTexture(m_renderer, "resources/SpriteSheet.png");
	if (sprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return NULL;
	}

	components.push_back(std::make_shared<GraphicsComponent>(m_renderer, 1000));

	return std::make_shared<GameActor>(world, components, Vector2D<int>(0, 0), Vector2D<int>(100, 100), sprite);
}

StrongGameActorPtr ActorFactory::CreateEnemy(StrongWorldPtr world)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<AIComponent>());
	
	auto sprite = loadTexture(m_renderer, "resources/SpriteSheet.png");
	if (sprite == NULL)
	{
		printf("Failed to load texture image!\n");
		return NULL;
	}

	components.push_back(std::make_shared<GraphicsComponent>(m_renderer, 6000));

	return std::make_shared<GameActor>(world, components, Vector2D<int>(200, 100), Vector2D<int>(100, 100), sprite);
}

StrongGameActorPtr ActorFactory::CreateCamera(StrongWorldPtr world, StrongGameActorPtr target)
{
	ComponentList components = ComponentList();
	components.push_back(std::make_shared<FollowTargetAIComponent>(target));

	return std::make_shared<GameActor>(world, components, Vector2D<int>(200, 100), Vector2D<int>(World::SCREEN_WIDTH, World::SCREEN_HEIGHT));
}

StrongActorComponentPtr ActorFactory::VCreateComponent(XMLElement* pData)
{
	const char* name = pData->Value();
	StrongActorComponentPtr pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

	// initialize the component if we found one
	if (pComponent)
	{
		if (!pComponent->Init(pData))
		{
			//GCC_ERROR("Component failed to initialize: " + std::string(name));
			return StrongActorComponentPtr();
		}
	}
	else
	{
		//GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return StrongActorComponentPtr();  // fail
	}

	// pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
	// custom CreateComponent() function in a sub class.
	return pComponent;
}


StrongActorComponentPtr ActorFactory::ActorComponentCreator()
{
	//StrongActorComponentPtr pActor();// new ActorComponent(GetNextComponentId()));

	return StrongActorComponentPtr();
}