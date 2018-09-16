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

		Vector2D<float> relMousePos = m_pOwner->GetMousePosition() + actorFactory->GetCurrentCamera()->GetTransformComponent()->GetPosition();
		relMousePos.y = World::SCREEN_HEIGHT - relMousePos.y;

		auto actorPos = m_pPhysicsComponent->GetTransformComponent()->GetPosition();
		Vector2D<float> dirVec = (relMousePos - actorPos).Normalize();
		//Vector2D<float> spawnPos = dirVec * 50;
		actorFactory->CreateProjectile(actorPos + (dirVec * 70), dirVec * 75.0f);
	}
}