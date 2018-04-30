#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include "PlayerComponents/PlayerInputComponent.h"
#include "PlayerComponents/PlayerMovementComponent.h"
#include "PlayerComponents/PlayerViewComponent.h"
#include "PlayerComponents/PlayerAnimationComponent.h"
#include "PkgBody.h"

class LocalPlayerComponent :public IEntityComponent
{
public:
	LocalPlayerComponent() = default;
    virtual ~LocalPlayerComponent() {}

    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<LocalPlayerComponent>& desc)
    {
        desc.SetGUID("{DA6C686E-A078-423e-B70B-8FE7B83E71C4}"_cry_guid);
        desc.SetEditorCategory("Game");
        desc.SetLabel("LocalPlayer");
        desc.SetDescription("Local player component.");
		desc.SetIcon("icons:ObjectTypes/object.ico");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Attach });
    }

    virtual uint64 GetEventMask() const override;
    // IEntityComponent
    virtual void Initialize() override;
    virtual void ProcessEvent(const SEntityEvent& event) override;
    virtual void OnShutDown() override;
    // ~IEntityComponent
public:
	const PkgBodyPlayerTransfrom GetPlayerTransform() const;
	const PkgBodyPlayerAnimation GetPlayerAnimations() const;
private:
    void UpdateMovementRequest(float frameTime);
    void UpdateLookDirectionRequest(float frameTime);
    void UpdateAnimation(float frameTime);

    void Reset();
private:
	PlayerInputComponent* m_playerInputComponent = nullptr;
	PlayerMovementComponent* m_playerMovementComponent = nullptr;
	PlayerViewComponent* m_playerViewComponent = nullptr;
	PlayerAnimationComponent* m_playerAnimationComponent = nullptr;
};
