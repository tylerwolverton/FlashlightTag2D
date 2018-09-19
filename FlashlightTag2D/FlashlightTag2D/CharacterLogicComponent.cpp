#include "CharacterLogicComponent.h"
#include "GameActor.h"
#include "ActorFactory.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "World.h"
#include "ServiceLocator.h"

#include <SDL.h>

CharacterLogicComponent::CharacterLogicComponent(ComponentId componentId, std::shared_ptr<PhysicsComponent> physicsComponent)
	: LogicComponent(componentId),
	  m_pPhysicsComponent(physicsComponent)
{
}

CharacterLogicComponent::~CharacterLogicComponent()
{
}

void CharacterLogicComponent::MoveUp()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, m_pPhysicsComponent->GetCurSpeed()));
}

void CharacterLogicComponent::MoveDown()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(0, -m_pPhysicsComponent->GetCurSpeed()));
}

void CharacterLogicComponent::MoveRight()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(m_pPhysicsComponent->GetCurSpeed(), 0));
}

void CharacterLogicComponent::MoveLeft()
{
	m_pPhysicsComponent->AddForce(Vector2D<float>(-m_pPhysicsComponent->GetCurSpeed(), 0));
}

void CharacterLogicComponent::Shoot()
{
	uint32_t curTicks = SDL_GetTicks();
	if (curTicks - lastTickVal > 500)
	{
		lastTickVal = curTicks;
		auto actorFactory = ServiceLocator::GetActorFactory();
		if (actorFactory == nullptr)
		{
			return;
		}

        std::shared_ptr<GameActor> actor = actorFactory->GetActor(m_parentActorId);
        if (actor == nullptr)
        {
            return;
        }

        // Need to add camera position to mouse position, since mouse pos will always be within screen bounds
        Vector2D<float> cameraPos = actorFactory->GetCurrentCamera()->GetTransformComponent()->GetPosition();
        Vector2D<float> relMousePos = Vector2D<float>(actor->GetMousePosition().x + cameraPos.x, actor->GetMousePosition().y - cameraPos.y);
        relMousePos.y = World::SCREEN_HEIGHT - relMousePos.y;

		auto actorPos = m_pPhysicsComponent->GetTransformComponent()->GetPosition();
		Vector2D<float> dirVec = (relMousePos - actorPos).Normalize();
		actorFactory->CreateProjectile(actorPos + (dirVec * 70), dirVec * 75.0f);
	}
}