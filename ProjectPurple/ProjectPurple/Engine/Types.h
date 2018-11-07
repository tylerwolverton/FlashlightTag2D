#pragma once
#include <memory>
#include <unordered_map>

#include "Vector2D.h"

class ActorComponent;

typedef unsigned long ActorId;
typedef unsigned long ComponentId;

static const int ComponentTypeCount = 8;

enum EComponentTypes
{
    AI,
    BaseLogic,
    Graphics,
    Input,
    Physics,
    Transform,
    GameState,
    Life
};

enum EComponentNames
{
    NoneComponentEnum,
    AIComponentEnum,
    LogicComponentEnum,
    CameraFollowComponentEnum,
    GraphicsComponentEnum,
    InputComponentEnum,
    PhysicsComponentEnum,
    TransformComponentEnum,
    GameStateComponentEnum,
    LifeComponentEnum
};

typedef std::unordered_map<EComponentNames, std::shared_ptr<ActorComponent>> ComponentMap;

enum EInputValues : uint32_t
{
    None = 0,
    Esc = 1 << 0,
    W = 1 << 1,
    A = 1 << 2,
    S = 1 << 3,
    D = 1 << 4,
    Space = 1 << 5,
    Return = 1 << 6,
    MouseLeft = 1 << 7,
    MouseRight = 1 << 8,
    MouseMiddle = 1 << 9,
    Up = 1 << 10,
    Left = 1 << 11,
    Down = 1 << 12,
    Right = 1 << 13
};

class InputData
{
public:
    uint32_t buttonsPressed;
    Vector2D<int> mousePos;
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