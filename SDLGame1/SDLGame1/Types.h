#pragma once
#include <memory>
#include <map>

class Actor;
class ActorComponent;

typedef ActorComponent *(*ActorComponentCreator)();
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

typedef unsigned long ActorId;
typedef unsigned long ComponentId;
typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;

const char* PlayerActor = "PlayerActor";