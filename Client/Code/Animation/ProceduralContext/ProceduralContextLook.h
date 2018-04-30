#pragma once

#include <ICryMannequin.h>	

class CProceduralContextLook : public IProceduralContext
{
public:
	PROCEDURAL_CONTEXT(CProceduralContextLook, "ProceduralContextLook", "0928592b-d916-48a5-9024-c8221945bb17"_cry_guid);

	CProceduralContextLook() {}
	virtual ~CProceduralContextLook() {}

	// IProceduralContext
	virtual void Initialise(IEntity& entity, IActionController& actionController) override;
	virtual void Update(float timePassedSeconds) override;
	// ~IProceduralContext

	void SetIsLookingGame(const bool isLooking) { m_isLookingGame = isLooking; }
	void SetIsLookingProcClip(const bool isLooking) { m_isLookingProcClip = isLooking; }
	void SetLookTarget(const Vec3& lookTarget) { m_playerLookTarget = lookTarget; }

	void SetBlendInTime(const float blendInTime);
	void SetBlendOutTime(const float blendOutTime);
	void SetFovRadians(const float fovRadians);

private:
	void InitialisePoseBlenderLook();
	void InitialiseGameLookTarget();

private:
	IAnimationPoseBlenderDir * m_poseBlenderLook = nullptr;

	bool m_isLookingGame = true;
	bool m_isLookingProcClip = true;
	Vec3 m_playerLookTarget = ZERO;
};

