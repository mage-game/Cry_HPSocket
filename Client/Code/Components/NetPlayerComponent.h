#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include "PlayerComponents/PlayerMovementComponent.h"
#include "PlayerComponents/PlayerAnimationComponent.h"
#include "PkgBody.h"

class NetPlayerComponent : public IEntityComponent
{
public:
    NetPlayerComponent() {}
    virtual ~NetPlayerComponent() {}

    // Reflect type to set a unique identifier for this component
    static void ReflectType(Schematyc::CTypeDesc<NetPlayerComponent>& desc)
    {
        desc.SetGUID("{D0506D28-3C66-4109-A65C-E86E5A07E2EE}"_cry_guid);
		desc.SetComponentFlags(IEntityComponent::EFlags::HiddenFromUser);
    }

    virtual uint64 GetEventMask() const override;
    // IEntityComponent
    virtual void Initialize() override;
    virtual void ProcessEvent(const SEntityEvent& event) override;
    virtual void OnShutDown() override;
    // ~IEntityComponent
public:
	void SetPlayerTransform(const PkgBodyPlayerTransfrom& transform);
	void SetPlayerAnimations(const PkgBodyPlayerAnimation& animationIDs);

private:
	void Reset();
	//void UpdateMovementRequest(float frameTime);
	//void UpdateAnimation(float frameTime);
private:
	PlayerMovementComponent* m_playerMovementComponent = nullptr;
	PlayerAnimationComponent* m_playerAnimationComponent = nullptr;
};
