#pragma once

#include <CryEntitySystem/IEntityComponent.h>				

#include <DefaultComponents/Physics/SampleRigidbodyActorComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>

#include "PlayerInputComponent.h"
 
class PlayerMovementComponent : public IEntityComponent
{	   	
public:
	PlayerMovementComponent() = default;
	virtual ~PlayerMovementComponent() {}

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<PlayerMovementComponent>& desc)
	{
		desc.SetGUID("{638AE340-775B-4014-BF8F-D56FCDEB94DC}"_cry_guid);
		//desc.SetEditorCategory("Players");
		desc.SetComponentFlags(IEntityComponent::EFlags::HiddenFromUser);
	}

	virtual uint64 GetEventMask() const override;
	// IEntityComponent
	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	//virtual void OnShutDown() override;
	// ~IEntityComponent

public:
	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();

	void Jump();

	void TurnLeft();
	void TurnRight();

	const bool IsWalking() const 
	{
		return m_isWalking;
	}
	const bool IsTurning() const
	{
		return m_isTurning;
	}
private:			 	
	void UpdateMovementRequest(float frameTime);
	void UpdateTurnRequest(float frameTime);
	void Reset();
private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_characterControllerComponen = nullptr;
	Cry::DefaultComponents::CSampleActorComponent* m_sampleActorComponent = nullptr;
	
	Vec3 m_moveVelocity = ZERO;		  	
	float m_turnAngles = 0.0f;

	bool m_isWalking = false;
	bool m_isTurning = false;
};
