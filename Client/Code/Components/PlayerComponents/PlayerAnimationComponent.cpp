#include "StdAfx.h"
#include "PlayerAnimationComponent.h"
#include "CrySchematyc\Env\Elements\EnvComponent.h"

uint64 PlayerAnimationComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void PlayerAnimationComponent::Initialize()
{
	m_animationComponent = GetEntity()->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	string rootNodename = "LocalPlayerProperty";
	string animationDatabase = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "database_file");
	string characterFile = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "character_file");
	string controllerDefinitionFile = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "controller_def_file");
	string defaultScopeContextName = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "default_scope_context_name");
	string idleFragment = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "default_fragement_name");
	string walkFragment = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "move_fragement_name");
	string rotateTag = mEnv->g_pGameConfiger->GetPropertyByName(rootNodename, "rotate_tag_name");

	m_animationComponent->SetMannequinAnimationDatabaseFile(animationDatabase);
	m_animationComponent->SetCharacterFile(characterFile);
	m_animationComponent->SetControllerDefinitionFile(controllerDefinitionFile);
	m_animationComponent->SetDefaultScopeContextName(defaultScopeContextName);
	m_animationComponent->SetDefaultFragmentName(idleFragment);
	m_animationComponent->SetAnimationDrivenMotion(false);
	m_animationComponent->LoadFromDisk();

	m_idleFragmentId = m_animationComponent->GetFragmentId(idleFragment);
	m_walkFragmentId = m_animationComponent->GetFragmentId(walkFragment);
	m_rotateTagId = m_animationComponent->GetTagId(rotateTag);

	Reset();
}

void PlayerAnimationComponent::ProcessEvent(const SEntityEvent& event)
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
		UpdateAnimation(frameTime);
	}
	break;
	}
}

void PlayerAnimationComponent::Reset()
{
	m_animationComponent->ResetCharacter();

	m_activeFragmentId = FRAGMENT_ID_INVALID;
}

void PlayerAnimationComponent::UpdateAnimation(float frameTime)
{
	if (auto *pPhysEnt = GetEntity()->GetPhysics())
	{
		// Update entity rotation as the player turns
		// Start with getting the look orientation's yaw, pitch and roll
		Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(GetEntity()->GetLocalTM()));

		// Re-calculate the quaternion based on the corrected look orientation
		Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

		// Get the player's velocity from physics
		pe_status_dynamics playerDynamics;
		if (pPhysEnt->GetStatus(&playerDynamics) != 0)
		{
			// Set turn rate as the difference between previous and new entity rotation
			float turnAngle = Ang3::CreateRadZ(GetEntity()->GetForwardDir(), correctedOrientation.GetColumn1()) / frameTime;
			float travelAngle = Ang3::CreateRadZ(GetEntity()->GetForwardDir(), playerDynamics.v.GetNormalized());
			float travelSpeed = playerDynamics.v.GetLength2D();

			// Set the travel speed based on the physics velocity magnitude
			// Keep in mind that the maximum number for motion parameters is 10.
			// If your velocity can reach a magnitude higher than this, divide by the maximum theoretical account and work with a 0 - 1 ratio.
			m_animationComponent->SetMotionParameter(eMotionParamID_TravelSpeed, travelSpeed);

			// Update the turn speed in CryAnimation, note that the maximum motion parameter (10) applies here too.
			m_animationComponent->SetMotionParameter(eMotionParamID_TurnAngle, turnAngle);
			m_animationComponent->SetMotionParameter(eMotionParamID_TravelAngle, travelAngle);

			pe_status_living livingStatus;
			if (pPhysEnt->GetStatus(&livingStatus) != 0)
			{
				// Store the ground normal in case it is needed
				// Note that users have to check if we're on ground before using, is considered invalid in air.
				if (!livingStatus.bFlying)
				{
					// Calculate slope value
					Vec3 groundNormal = livingStatus.groundSlope * correctedOrientation;
					groundNormal.x = 0.0f;
					float cosine = Vec3Constants<float>::fVec3_OneZ | groundNormal;
					Vec3 sine = Vec3Constants<float>::fVec3_OneZ % groundNormal;

					float travelSlope = atan2f(sgn(sine.x) * sine.GetLength(), cosine);

					m_animationComponent->SetMotionParameter(eMotionParamID_TravelSlope, travelSlope);
				}
			}				
		}
		m_animationComponent->SetTagWithId(m_rotateTagId, m_isTurning);
	}

	FragmentID desiredFragmentId = m_isWalking ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId)
	{
		m_activeFragmentId = desiredFragmentId;
		m_animationComponent->QueueFragmentWithId(m_activeFragmentId);
	}
}

AUTO_REGISTER_COMPONENT(PlayerAnimationComponent)
