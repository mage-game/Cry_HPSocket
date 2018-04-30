#pragma once

#include <ICryMannequin.h>	
#include "ProceduralContextHelpers.h"

class CProceduralContextAim
	: public IProceduralContext
{
public:
	PROCEDURAL_CONTEXT(CProceduralContextAim, "ProceduralContextAim", "4a5625bb-01d1-49c6-b563-2cf301b58e38"_cry_guid);

	CProceduralContextAim() {}
	virtual ~CProceduralContextAim() {}

	// IProceduralContext
	virtual void Initialise(IEntity& entity, IActionController& actionController) override;
	virtual void Update(float timePassedSeconds) override;
	// ~IProceduralContext

	void SetIsAimingGame(const bool isAiming) { m_isAimingGame = isAiming; }
	void SetIsAimingProcClip(const bool isAiming) { m_isAimingProcClip = isAiming; }
	void SetAimTarget(const Vec3& aimTarget) { m_aimTarget = aimTarget; }

	void SetBlendInTime(const float blendInTime);
	void SetBlendOutTime(const float blendOutTime);

	uint32 RequestPolarCoordinatesSmoothingParameters(const Vec2& maxSmoothRateRadiansPerSecond, const float smoothTimeSeconds);
	void CancelPolarCoordinatesSmoothingParameters(const uint32 requestId);

private:
	void InitialisePoseBlenderAim();
	void InitialiseGameAimTarget();

	void UpdatePolarCoordinatesSmoothingParameters();

private:
	IAnimationPoseBlenderDir * m_poseBlenderAim = nullptr;
	bool m_isAimingGame = true;
	bool m_isAimingProcClip = false;
	Vec3 m_aimTarget = ZERO;

	Vec2 m_polarCoordinatesMaxSmoothRateRadiansPerSecond = DEG2RAD(Vec2(360.0f, 360.0f));
	float m_polarCoordinatesSmoothTimeSeconds = 0.1f;

	struct SPolarCoordinatesSmoothingParametersRequest
	{
		uint32 id;
		Vec2 maxSmoothRateRadiansPerSecond;
		float smoothTimeSeconds;
	};
	typedef ProceduralContextHelpers::CRequestList< SPolarCoordinatesSmoothingParametersRequest > TPolarCoordinatesSmoothingParametersRequestList;
	TPolarCoordinatesSmoothingParametersRequestList m_polarCoordinatesSmoothingParametersRequestList;
};

