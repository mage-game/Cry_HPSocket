#pragma once
				  
#include <CryEntitySystem/IEntityComponent.h>

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

enum AnimationFragmentID
{
	IDLE = BIT(0),
	WALK = BIT(1),
	ROTATE = BIT(2)
};

class PlayerAnimationComponent : public IEntityComponent
{
public:
	PlayerAnimationComponent() = default;
	virtual ~PlayerAnimationComponent() {}

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<PlayerAnimationComponent>& desc)
	{
		desc.SetGUID("{8BB42E3F-7C01-4be3-A49E-159944E2F812}"_cry_guid);
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
	void SetIsWalking(bool isWalking)
	{
		m_isWalking = isWalking;
	}
	void SetIsTurning(bool isTurning)
	{
		m_isTurning = isTurning;
	}

	uint64_t GetAnimationIDs() const
	{
		return m_activeFragmentId;
	}
private:
	void UpdateAnimation(float frameTime);
	void Reset();
private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_animationComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	FragmentID m_activeFragmentId;

	bool m_isWalking = false;
	bool m_isTurning = false;
};
