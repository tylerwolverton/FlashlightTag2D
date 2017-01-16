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

//const char* PlayerActor = "PlayerActor";


static std::map<const char*, ComponentId> ComponentIdNameMap =
{
	{"NoneComponent",  0},
	{"InputComponent", 1}
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

// GenericObjectFactory
template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return new SubType; }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
	typedef BaseClass* (*ObjectCreationFunction)(void);
	std::map<IdType, ObjectCreationFunction> m_creationFunctions;

public:
	template <class SubClass>
	bool Register(IdType id)
	{
		auto findIt = m_creationFunctions.find(id);
		if (findIt == m_creationFunctions.end())
		{
			m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
			return true;
		}

		return false;
	}

	BaseClass* Create(IdType id)
	{
		auto findIt = m_creationFunctions.find(id);
		if (findIt != m_creationFunctions.end())
		{
			ObjectCreationFunction pFunc = findIt->second;
			return pFunc();
		}

		return NULL;
	}
};