#include "StdAfx.h"
#include "PlayerInputComponent.h"
#include "CrySchematyc\Env\Elements\EnvComponent.h"

void PlayerInputComponent::Initialize()
{
	m_inputComponent = GetEntity()->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	// Move left.
	m_inputComponent->RegisterAction("player", "move_left", [this](int activationMode, float value)
	{ 
		InputFlag inputFlag(EInputFlag::MoveLeft);
		HandleInputFlagChange(inputFlag, activationMode);
	});
	m_inputComponent->BindAction("player", "move_left", eAID_KeyboardMouse, EKeyId::eKI_A);

	// Move right.
	m_inputComponent->RegisterAction("player", "move_right", [this](int activationMode, float value) 
	{ 
		InputFlag inputFlag(EInputFlag::MoveRight);
		HandleInputFlagChange(inputFlag, activationMode);
	});
	m_inputComponent->BindAction("player", "move_right", eAID_KeyboardMouse, EKeyId::eKI_D);

	// Move forward.
	m_inputComponent->RegisterAction("player", "move_forward", [this](int activationMode, float value) 
	{ 
		InputFlag inputFlag(EInputFlag::MoveForward);
		HandleInputFlagChange(inputFlag, activationMode);
	});
	m_inputComponent->BindAction("player", "move_forward", eAID_KeyboardMouse, EKeyId::eKI_W);

	// Move backward.
	m_inputComponent->RegisterAction("player", "move_backward", [this](int activationMode, float value) 
	{
		InputFlag inputFlag(EInputFlag::MoveBack);
		HandleInputFlagChange(inputFlag, activationMode);
	});
	m_inputComponent->BindAction("player", "move_backward", eAID_KeyboardMouse, EKeyId::eKI_S);

	// Turn left.
	m_inputComponent->RegisterAction("player", "turn_left", [this](int activationMode, float value)
	{
		InputFlag inputFlag(EInputFlag::TurnLeft);
		HandleInputFlagChange(inputFlag, activationMode);
	});
	m_inputComponent->BindAction("player", "turn_left", eAID_KeyboardMouse, EKeyId::eKI_Q);

	// Turn right.
	m_inputComponent->RegisterAction("player", "turn_right", [this](int activationMode, float value)
	{
		InputFlag inputFlag(EInputFlag::TurnRight);
		HandleInputFlagChange(inputFlag, activationMode);
	});
	m_inputComponent->BindAction("player", "turn_right", eAID_KeyboardMouse, EKeyId::eKI_E);

	// Jump.
	m_inputComponent->RegisterAction("player", "move_jump", [this](int activationMode, float value) 
	{ 
		//OnActionJump(activationMode, value); 
		InputFlag inputFlag(EInputFlag::Jump);
		HandleInputFlagChange(inputFlag, activationMode);
	});	 
	m_inputComponent->BindAction("player", "move_jump", eAID_KeyboardMouse, EKeyId::eKI_Space);
}

void PlayerInputComponent::HandleInputFlagChange(InputFlag flags, int activationMode)
{
	if (activationMode == eIS_Released)
	{
		m_inputFlags &= ~flags;
	}
	else
	{
		m_inputFlags |= flags;
	}
}

void PlayerInputComponent::OnActionJump(int activationMode, float value)
{
	if (activationMode == eAAM_OnPress)
	{
		CryLogAlways("Player jumped");
	}	     
}

AUTO_REGISTER_COMPONENT(PlayerInputComponent)
