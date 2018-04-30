#include "StdAfx.h"
#include "LocalPlayerComponent.h"
#include "GameConfiger.h"
#include "CrySchematyc\Env\Elements\EnvComponent.h"

uint64 LocalPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void LocalPlayerComponent::Initialize()
{
	m_playerInputComponent = GetEntity()->GetOrCreateComponentClass<PlayerInputComponent>();
	m_playerMovementComponent = GetEntity()->GetOrCreateComponentClass<PlayerMovementComponent>();
	//m_pPlayerViewComponent = GetEntity()->GetOrCreateComponentClass<PlayerViewComponent>();
	m_playerAnimationComponent = GetEntity()->GetOrCreateComponentClass<PlayerAnimationComponent>();
}

void LocalPlayerComponent::ProcessEvent(const SEntityEvent& event)
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
        UpdateLookDirectionRequest(frameTime); 
        UpdateAnimation(frameTime);  
    }
    break;
    }
}

void LocalPlayerComponent::OnShutDown()
{

}

const PkgBodyPlayerTransfrom LocalPlayerComponent::GetPlayerTransform() const
{
	Vec3 pos = GetEntity()->GetWorldPos();
	Ang3 rot = Ang3(GetEntity()->GetWorldAngles());

	PkgBodyPlayerTransfrom transform;
	transform.posX = pos.x;
	transform.posY = pos.y;
	transform.posZ = pos.z;
	transform.posX = rot.x;
	transform.posY = rot.y;
	transform.posZ = rot.z;

	return transform;
}

const PkgBodyPlayerAnimation LocalPlayerComponent::GetPlayerAnimations() const
{
	PkgBodyPlayerAnimation animation;
	animation.animationIDs = m_playerAnimationComponent->GetAnimationIDs();
	return animation;
}

void LocalPlayerComponent::UpdateMovementRequest(float frameTime)
{
	InputFlag inputFlags = m_playerInputComponent->GetInputFlags();
	if (inputFlags & EInputFlag::MoveLeft)
	{
		m_playerMovementComponent->MoveLeft();
	}
	if (inputFlags & EInputFlag::MoveRight)
	{
		m_playerMovementComponent->MoveRight();
	}
	if (inputFlags & EInputFlag::MoveForward)
	{
		m_playerMovementComponent->MoveForward();
	}
	if (inputFlags & EInputFlag::MoveBack)
	{
		m_playerMovementComponent->MoveBack();
	}
	if (inputFlags & EInputFlag::TurnLeft)
	{
		m_playerMovementComponent->TurnLeft();
	}
	if (inputFlags & EInputFlag::TurnRight)
	{
		m_playerMovementComponent->TurnRight();
	}
	if (inputFlags & EInputFlag::Jump)
	{
		m_playerMovementComponent->Jump();
	}
}

void LocalPlayerComponent::UpdateLookDirectionRequest(float frameTime)
{
	//const Vec2& mouseRotation = m_pPlayerInputComponent->GetMouseRotation();
	//m_pPlayerViewComponent->SetMouseDeltaRotation(mouseRotation);
}

void LocalPlayerComponent::UpdateAnimation(float frameTime)
{
	bool isWalking = m_playerMovementComponent->IsWalking();
	m_playerAnimationComponent->SetIsWalking(isWalking);

	bool isTurning = m_playerMovementComponent->IsTurning();
	m_playerAnimationComponent->SetIsTurning(isTurning);
}

void LocalPlayerComponent::Reset()
{
	GetEntity()->Hide(false);
	//GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));
}

AUTO_REGISTER_COMPONENT(LocalPlayerComponent)
