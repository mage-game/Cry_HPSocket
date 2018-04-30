#include "StdAfx.h"
#include "PlayerViewComponent.h"
#include "CrySchematyc\Env\Elements\EnvComponent.h"

uint64 PlayerViewComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void PlayerViewComponent::Initialize()
{
	m_cameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
}

void PlayerViewComponent::ProcessEvent(const SEntityEvent& event)
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
		UpdateLookDirectionRequest(frameTime);
		UpdateCamera(frameTime);
	}
	break;
	}
}

void PlayerViewComponent::UpdateLookDirectionRequest(float frameTime)
{
	const float rotationSpeed = 0.002f;
	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	m_horizontalAngularVelocity = (m_mouseDeltaRotation.x * rotationSpeed) / frameTime;

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));  
	// Yaw
	ypr.x += m_mouseDeltaRotation.x * rotationSpeed;
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);
	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset the mouse delta accumulator every frame
	m_mouseDeltaRotation = ZERO;
}

void PlayerViewComponent::UpdateCamera(float frameTime)
{
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	const float viewOffsetForward = -1.5f;
	const float viewOffsetUp = 2.f;
	localTransform.SetTranslation(Vec3(0, viewOffsetForward, viewOffsetUp));

	m_cameraComponent->SetTransformMatrix(localTransform);
}

void PlayerViewComponent::Reset()
{
	m_lookOrientation = IDENTITY;
	m_horizontalAngularVelocity = 0.0f;
}

//AUTO_REGISTER_COMPONENT(PlayerViewComponent)
