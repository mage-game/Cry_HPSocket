#include "StdAfx.h"
#include "PlayerMovementComponent.h"
#include "CrySchematyc\Env\Elements\EnvComponent.h"

const float DEFAULT_MOVE_SPEED = 20.0f;
const float DEFAULT_TURN_SPEED = 10.0f;

uint64 PlayerMovementComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void PlayerMovementComponent::Initialize()
{
	m_characterControllerComponen = GetEntity()->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	//m_pCSampleActorComponent = GetEntity()->GetOrCreateComponent<Cry::DefaultComponents::CSampleActorComponent>();
}

void PlayerMovementComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_START_GAME:
	{
		Reset();
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		float frameTime = pCtx->fFrameTime;
		UpdateMovementRequest(frameTime);
		UpdateTurnRequest(frameTime);
	}
	break;
	}
}

void PlayerMovementComponent::MoveForward()
{
	m_moveVelocity.y += DEFAULT_MOVE_SPEED;
}

void PlayerMovementComponent::MoveBack()
{
	m_moveVelocity.y -= DEFAULT_MOVE_SPEED;
}

void PlayerMovementComponent::MoveLeft()
{
	m_moveVelocity.x -= DEFAULT_MOVE_SPEED;
}

void PlayerMovementComponent::MoveRight()
{
	m_moveVelocity.x += DEFAULT_MOVE_SPEED;
}

void PlayerMovementComponent::Jump()
{
	//m_pCSampleActorComponent->Jump(2.0f);
}

void PlayerMovementComponent::TurnLeft()
{
	m_turnAngles += DEFAULT_TURN_SPEED;
}

void PlayerMovementComponent::TurnRight()
{
	m_turnAngles -= DEFAULT_TURN_SPEED;
}

void PlayerMovementComponent::UpdateMovementRequest(float frameTime)
{
	if (!m_characterControllerComponen->IsOnGround())
		return;					 

	if (m_moveVelocity.len2()>0.0001f)
	{
		Vec3 velocity = GetEntity()->GetWorldRotation() * m_moveVelocity*frameTime;
		m_characterControllerComponen->AddVelocity(velocity);

		m_moveVelocity = ZERO;
	} 			
	m_isWalking = m_characterControllerComponen->IsWalking();
}

void PlayerMovementComponent::UpdateTurnRequest(float frameTime)
{
	m_isTurning = fabs_tpl(m_turnAngles) > 0.0001f;
	if (m_isTurning)
	{
		Quat rotation = Quat::CreateRotationZ(m_turnAngles*frameTime)*GetEntity()->GetRotation();
		GetEntity()->SetRotation(rotation);

		m_turnAngles = 0.0f;
	}
}

void PlayerMovementComponent::Reset()
{
	m_moveVelocity = ZERO;
	m_turnAngles = 0.0f;
	m_characterControllerComponen->Physicalize();
	//m_pCSampleActorComponent->Physicalize();
}

AUTO_REGISTER_COMPONENT(PlayerMovementComponent)
