#include "StdAfx.h"
#include "ProceduralContextAim.h"	  

CRYREGISTER_CLASS(CProceduralContextAim);

void CProceduralContextAim::Initialise(IEntity& entity, IActionController& actionController)
{
	IProceduralContext::Initialise(entity, actionController);

	InitialisePoseBlenderAim();
	InitialiseGameAimTarget();
}

void CProceduralContextAim::InitialisePoseBlenderAim()
{
	CRY_ASSERT(m_entity);

	const int slot = 0;
	ICharacterInstance* pCharacterInstance = m_entity->GetCharacter(slot);
	if (pCharacterInstance == NULL)
	{
		return;
	}

	ISkeletonPose* pSkeletonPose = pCharacterInstance->GetISkeletonPose();
	if (pSkeletonPose == NULL)
	{
		return;
	}

	m_poseBlenderAim = pSkeletonPose->GetIPoseBlenderAim();

	if (m_poseBlenderAim)
	{
		m_polarCoordinatesSmoothTimeSeconds = 0.1f;
		float polarCoordinatesMaxYawDegreesPerSecond = 360.f;
		float polarCoordinatesMaxPitchDegreesPerSecond = 360.f;
		float fadeInSeconds = 0.25f;
		float fadeOutSeconds = 0.25f;
		float fadeOutMinDistance = 0.f;

		m_polarCoordinatesMaxSmoothRateRadiansPerSecond = Vec2(DEG2RAD(polarCoordinatesMaxYawDegreesPerSecond), DEG2RAD(polarCoordinatesMaxPitchDegreesPerSecond));

		m_poseBlenderAim->SetPolarCoordinatesSmoothTimeSeconds(m_polarCoordinatesSmoothTimeSeconds);
		m_poseBlenderAim->SetPolarCoordinatesMaxRadiansPerSecond(m_polarCoordinatesMaxSmoothRateRadiansPerSecond);
		m_poseBlenderAim->SetFadeInSpeed(fadeInSeconds);
		m_poseBlenderAim->SetFadeOutSpeed(fadeOutSeconds);
		m_poseBlenderAim->SetFadeOutMinDistance(fadeOutMinDistance);
		m_poseBlenderAim->SetState(false);
	}
}

void CProceduralContextAim::InitialiseGameAimTarget()
{
	m_aimTarget = (m_entity->GetForwardDir() * 10.0f) + m_entity->GetWorldPos();
	if (m_poseBlenderAim)
	{
		m_poseBlenderAim->SetTarget(m_aimTarget);
	}
}

void CProceduralContextAim::Update(float timePassedSeconds)
{
	if (m_poseBlenderAim == NULL)
	{
		const bool canAim = (m_isAimingGame && m_isAimingProcClip);
		m_poseBlenderAim->SetState(canAim);

		UpdatePolarCoordinatesSmoothingParameters();

		if (canAim)
		{
			m_poseBlenderAim->SetTarget(m_aimTarget);
		}
	}
}

void CProceduralContextAim::UpdatePolarCoordinatesSmoothingParameters()
{
	Vec2 polarCoordinatesMaxSmoothRateRadiansPerSecond = m_polarCoordinatesMaxSmoothRateRadiansPerSecond;
	float polarCoordinatesSmoothTimeSeconds = m_polarCoordinatesSmoothTimeSeconds;

	const size_t requestCount = m_polarCoordinatesSmoothingParametersRequestList.GetCount();
	if (0 < requestCount)
	{
		const SPolarCoordinatesSmoothingParametersRequest& request = m_polarCoordinatesSmoothingParametersRequestList.GetRequest(requestCount - 1);

		polarCoordinatesMaxSmoothRateRadiansPerSecond = request.maxSmoothRateRadiansPerSecond;
		polarCoordinatesSmoothTimeSeconds = request.smoothTimeSeconds;
	}

	m_poseBlenderAim->SetPolarCoordinatesMaxRadiansPerSecond(polarCoordinatesMaxSmoothRateRadiansPerSecond);
	m_poseBlenderAim->SetPolarCoordinatesSmoothTimeSeconds(polarCoordinatesSmoothTimeSeconds);
}

void CProceduralContextAim::SetBlendInTime(const float blendInTime)
{
	if (m_poseBlenderAim)
	{
		m_poseBlenderAim->SetFadeInSpeed(blendInTime);
	}
}

void CProceduralContextAim::SetBlendOutTime(const float blendOutTime)
{
	if (m_poseBlenderAim)
	{
		m_poseBlenderAim->SetFadeOutSpeed(blendOutTime);
	}
}

uint32 CProceduralContextAim::RequestPolarCoordinatesSmoothingParameters(const Vec2& maxSmoothRateRadiansPerSecond, const float smoothTimeSeconds)
{
	SPolarCoordinatesSmoothingParametersRequest request;
	request.maxSmoothRateRadiansPerSecond = maxSmoothRateRadiansPerSecond;
	request.smoothTimeSeconds = smoothTimeSeconds;

	return m_polarCoordinatesSmoothingParametersRequestList.AddRequest(request);
}

void CProceduralContextAim::CancelPolarCoordinatesSmoothingParameters(const uint32 requestId)
{
	m_polarCoordinatesSmoothingParametersRequestList.RemoveRequest(requestId);
}
