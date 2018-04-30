#include "StdAfx.h"
#include "NetPlayerComponent.h"
#include "GameConfiger.h"

uint64 NetPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void NetPlayerComponent::Initialize()
{
	m_playerMovementComponent = GetEntity()->GetOrCreateComponentClass<PlayerMovementComponent>();
	m_playerAnimationComponent = GetEntity()->GetOrCreateComponentClass<PlayerAnimationComponent>();
}

void NetPlayerComponent::ProcessEvent(const SEntityEvent& event)
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

	}
	break;
	}
}

void NetPlayerComponent::OnShutDown()
{

}

void NetPlayerComponent::SetPlayerTransform(const PkgBodyPlayerTransfrom& transform)
{
	Vec3 pos = Vec3(transform.posX, transform.posY, transform.posZ);
	Ang3 rot = Ang3(transform.rotX, transform.rotY, transform.rotZ);

	GetEntity()->SetPosRotScale(pos, Quat(rot), Vec3(1.0f, 1.0f, 1.0f));
}

void NetPlayerComponent::SetPlayerAnimations(const PkgBodyPlayerAnimation& animationIDs)
{

}

void NetPlayerComponent::Reset()
{

}
