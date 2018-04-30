// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

//
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "ICryMannequin.h"
#include <CryAnimation/ICryAnimation.h>
#include <CryAnimation/IAnimationPoseModifier.h>
#include <CryExtension/ClassWeaver.h>
#include "../ProceduralContext/ProceduralContextLook.h"



class CProceduralClipLooking
	: public TProceduralContextualClip< CProceduralContextLook, SNoProceduralParams >
{
public:
	virtual void OnEnter(float blendTime, float duration, const SNoProceduralParams& params) override
	{
		m_context->SetBlendInTime(blendTime);
		m_context->SetIsLookingProcClip(true);
	}

	virtual void OnExit(float blendTime) override
	{
		m_context->SetBlendOutTime(blendTime);
		m_context->SetIsLookingProcClip(false);
	}

	virtual void Update(float timePassed) override
	{
		m_context->SetIsLookingProcClip(true);
	}
};

REGISTER_PROCEDURAL_CLIP(CProceduralClipLooking, "Looking");
