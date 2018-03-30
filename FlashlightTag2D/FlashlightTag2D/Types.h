#pragma once
#include <memory>
#include <unordered_map>
#include <list>

class Actor;
class ActorFactory;
class ActorComponent;
class Behavior;
class GameActor;
class Command;
class World;
class AIComponent;
class BaseLogicComponent;
class FollowTargetAIComponent;
class GraphicsComponent;
class InputComponent;
class PhysicsComponent;
class AIComponent;
class TransformComponent;
class GameStateComponent;

class InputManager;
class PhysicsManager;
class GraphicsManager;

class Texture2D;

typedef unsigned long ActorId;
typedef unsigned long ComponentId;

typedef std::shared_ptr<Actor>                   StrongActorPtr;
typedef std::shared_ptr<ActorFactory>            StrongActorFactoryPtr;
typedef std::shared_ptr<ActorComponent>          StrongActorComponentPtr;
typedef std::shared_ptr<AIComponent>             StrongAIComponentPtr;
typedef std::shared_ptr<BaseLogicComponent>      StrongBaseLogicComponentPtr;
typedef std::shared_ptr<FollowTargetAIComponent> StrongFollowTargetAIComponentPtr;
typedef std::shared_ptr<GraphicsComponent>       StrongGraphicsComponentPtr;
typedef std::shared_ptr<InputComponent>          StrongInputComponentPtr;
typedef std::shared_ptr<PhysicsComponent>        StrongPhysicsComponentPtr;
typedef std::shared_ptr<TransformComponent>      StrongTransformComponentPtr;
typedef std::shared_ptr<GameStateComponent>      StrongGameStateComponentPtr;

typedef std::shared_ptr<InputManager>            StrongInputManagerPtr;
typedef std::shared_ptr<PhysicsManager>          StrongPhysicsManagerPtr;
typedef std::shared_ptr<GraphicsManager>         StrongGraphicsManagerPtr;

typedef std::shared_ptr<Texture2D>               StrongTexture2DPtr;

typedef std::shared_ptr<GameActor> StrongGameActorPtr;
typedef std::shared_ptr<World> StrongWorldPtr;

typedef std::list<std::shared_ptr<GameActor>> StrongGameActorPtrList;

typedef std::list<std::shared_ptr<Behavior>> BehaviorList;
typedef std::list<std::shared_ptr<Command>> CommandList;
typedef std::shared_ptr<CommandList> CommandListPtr;
typedef std::list<StrongActorComponentPtr> ComponentList;

static const int ComponentTypeCount = 7;

enum EComponentTypes
{
	AI,
	BaseLogic,
	Graphics,
	Input,
	Physics,
	Transform,
	GameState
};

enum EComponentNames
{
	NoneComponentEnum,
	AIComponentEnum,
	BaseLogicComponentEnum,
	FollowTargetAIComponentEnum,
	GraphicsComponentEnum,
	InputComponentEnum,
	PhysicsComponentEnum,
	TransformComponentEnum,
	GameStateComponentEnum
};

typedef std::unordered_map <EComponentNames, StrongActorComponentPtr> ComponentMap;

enum EInputValues : uint32_t
{
	None = 0,
	Esc = 1 << 0,
	W = 1 << 1,
	A = 1 << 2,
	S = 1 << 3,
	D = 1 << 4
};

// ----------------------------------- templates ----------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
// [rez] Tr1 / Boost smart pointers make me sad.  In order to dereference a weak_ptr, you have to cast it to a 
// shared_ptr first.  You still have to check to see if the pointer is dead dead by calling expired() on the weak_ptr, 
// so why not just allow the weak_ptr to be dereferenceable?  It doesn't buy anything to force this extra step because 
// you can still cast a dead weak_ptr to a shared_ptr and crash.  Nice.  Anyway, this function takes some of that 
// headache away.
//---------------------------------------------------------------------------------------------------------------------
template <class Type>
std::shared_ptr<Type> MakeStrongPtr(std::weak_ptr<Type> pWeakPtr)
{
	if (!pWeakPtr.expired())
		return std::shared_ptr<Type>(pWeakPtr);
	else
		return std::shared_ptr<Type>();
}