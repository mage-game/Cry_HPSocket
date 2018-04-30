#include "StdAfx.h"
#include "ProceduralContextLook.h"

CRYREGISTER_CLASS(CProceduralContextLook);

void CProceduralContextLook::Initialise(IEntity& entity, IActionController& actionController)
{
	IProceduralContext::Initialise(entity, actionController);

	InitialisePoseBlenderLook();
	InitialiseGameLookTarget();
}

void CProceduralContextLook::InitialisePoseBlenderLook()
{
	if (ICharacterInstance* pCharacterInstance = m_entity->GetCharacter(0))
	{
		if (ISkeletonPose* pSkeletonPose = pCharacterInstance->GetISkeletonPose())
		{
			if (m_poseBlenderLook = pSkeletonPose->GetIPoseBlenderLook())
			{
				float polarCoordinatesSmoothTimeSeconds = 0.1f;
				float polarCoordinatesMaxYawDegreesPerSecond = 360.f;
				float polarCoordinatesMaxPitchDegreesPerSecond = 360.f;
				float fadeInSeconds = 0.25f;
				float fadeOutSeconds = 0.25f;
				float fadeOutMinDistance = 0.f;

				m_poseBlenderLook->SetPolarCoordinatesSmoothTimeSeconds(polarCoordinatesSmoothTimeSeconds);
				m_poseBlenderLook->SetPolarCoordinatesMaxRadiansPerSecond(Vec2(DEG2RAD(polarCoordinatesMaxYawDegreesPerSecond), DEG2RAD(polarCoordinatesMaxPitchDegreesPerSecond)));
				m_poseBlenderLook->SetFadeInSpeed(fadeInSeconds);
				m_poseBlenderLook->SetFadeOutSpeed(fadeOutSeconds);
				m_poseBlenderLook->SetFadeOutMinDistance(fadeOutMinDistance);
				m_poseBlenderLook->SetState(false);
			}
		}
	}
}

void CProceduralContextLook::InitialiseGameLookTarget()
{
	m_playerLookTarget = (m_entity->GetForwardDir() * 10.0f) + m_entity->GetWorldPos();
	if (m_poseBlenderLook)
	{
		m_poseBlenderLook->SetTarget(m_playerLookTarget);
	}
}

void CProceduralContextLook::Update(float timePassedSeconds)
{
	if (m_poseBlenderLook)
	{
		const bool canLook = (m_isLookingGame && m_isLookingProcClip);
		m_poseBlenderLook->SetState(canLook);
		if (canLook)
		{
			m_poseBlenderLook->SetTarget(m_playerLookTarget);
		}
	}
}

void CProceduralContextLook::SetBlendInTime(const float blendInTime)
{
	if (m_poseBlenderLook)
	{
		m_poseBlenderLook->SetFadeInSpeed(blendInTime);
	}
}

void CProceduralContextLook::SetBlendOutTime(const float blendOutTime)
{
	if (m_poseBlenderLook)
	{
		m_poseBlenderLook->SetFadeOutSpeed(blendOutTime);
	}
}

void CProceduralContextLook::SetFovRadians(const float fovRadians)
{
	if (m_poseBlenderLook)
	{
		m_poseBlenderLook->SetFadeoutAngle(fovRadians);
	}
}
