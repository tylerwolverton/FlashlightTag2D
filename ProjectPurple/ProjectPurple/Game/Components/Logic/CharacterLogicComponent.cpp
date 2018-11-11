#include "CharacterLogicComponent.h"
#include "GameActor.h"
#include "ActorFactory.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "World.h"
#include "ServiceLocator.h"

#include <SDL.h>

CharacterLogicComponent::CharacterLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsCompPtr)
    : LogicComponent(componentId),
      m_physicsCompPtr(physicsCompPtr)
{
}

CharacterLogicComponent::~CharacterLogicComponent()
{
}

void CharacterLogicComponent::MoveUp()
{
    m_physicsCompPtr->AddForce(Vector2D<float>(0, m_physicsCompPtr->GetCurSpeed()));
}

void CharacterLogicComponent::MoveDown()
{
    m_physicsCompPtr->AddForce(Vector2D<float>(0, -m_physicsCompPtr->GetCurSpeed()));
}

void CharacterLogicComponent::MoveRight()
{
    m_physicsCompPtr->AddForce(Vector2D<float>(m_physicsCompPtr->GetCurSpeed(), 0));
}

void CharacterLogicComponent::MoveLeft()
{
    m_physicsCompPtr->AddForce(Vector2D<float>(-m_physicsCompPtr->GetCurSpeed(), 0));
}

void CharacterLogicComponent::Shoot()
{
    uint32_t curTicks = SDL_GetTicks();
    if (curTicks - m_lastTickVal > 400)
    {
        m_lastTickVal = curTicks;
        auto actorFactoryPtr = ServiceLocator::GetActorFactory();
        if (actorFactoryPtr == nullptr)
        {
            return;
        }

        std::shared_ptr<GameActor> actor = actorFactoryPtr->GetActor(m_parentActorId);
        if (actor == nullptr)
        {
            return;
        }

        // Need to add camera position to mouse position, since mouse pos will always be within screen bounds
        Vector2D<float> cameraPos = actorFactoryPtr->GetCurrentCamera()->GetTransformCompPtr()->GetPosition();
        Vector2D<float> relMousePos = Vector2D<float>(actor->GetMousePosition().x + cameraPos.x, actor->GetMousePosition().y - cameraPos.y);
        relMousePos.y = World::SCREEN_HEIGHT - relMousePos.y;

		Vector2D<float> actorPos = m_physicsCompPtr->GetTransformCompPtr()->GetPosition();
        Vector2D<float> dirVec = (relMousePos - actorPos).Normalize();
        actorFactoryPtr->CreateProjectile(actorPos + (dirVec * 70), dirVec * 15.0f);
    }
}