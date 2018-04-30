#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include "PlayerInputComponent.h"
#include <DefaultComponents/Cameras/CameraComponent.h>

class PlayerViewComponent : public IEntityComponent
{
public:
	PlayerViewComponent() = default;
	virtual ~PlayerViewComponent() {}

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<PlayerViewComponent>& desc)
	{
		desc.SetGUID("{4231A3D3-7282-48c1-ADE2-4FC871388D92}"_cry_guid);
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
	void SetMouseDeltaRotation(const Vec2& mouseDeltaRotation)
	{
		m_mouseDeltaRotation = mouseDeltaRotation;
	}
private:
	void UpdateLookDirectionRequest(float frameTime);
	void UpdateCamera(float frameTime);
	void Reset();
private:
	Cry::DefaultComponents::CCameraComponent* m_cameraComponent = nullptr;

	Vec2 m_mouseDeltaRotation;
	Quat m_lookOrientation;
	float m_horizontalAngularVelocity;
};
