#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include <DefaultComponents/Input/InputComponent.h>

enum class EInputFlagType : uint32_t
{
    Hold = 0,
    Toggle
};

enum class EInputFlag : uint32_t
{
	None = 0,
	MoveLeft = BIT(0),
	MoveRight = BIT(1),
	MoveForward = BIT(2),
	MoveBack = BIT(3),
	TurnLeft = BIT(4),
	TurnRight = BIT(5),
	Jump = BIT(6)
};

typedef CEnumFlags<EInputFlag> InputFlag;

class PlayerInputComponent: public IEntityComponent
{           
public:
    PlayerInputComponent() = default;
    virtual ~PlayerInputComponent() {};

    static void ReflectType(Schematyc::CTypeDesc<PlayerInputComponent>& desc)
    {
        desc.SetGUID("{B38ECC98-FEB2-467D-B78B-05743B1CA260}"_cry_guid);
		//desc.SetEditorCategory("Players");
		desc.SetComponentFlags(IEntityComponent::EFlags::HiddenFromUser);
    }

    //virtual uint64 GetEventMask() const override;
    // IEntityComponent
    virtual void Initialize() override;	
	//virtual void ProcessEvent(const SEntityEvent& event) override;
    //virtual void OnShutDown() override;
    // ~IEntityComponent	
    
public:
	const InputFlag GetInputFlags() const
	{ 
		return m_inputFlags;
	}
private:
    void HandleInputFlagChange(InputFlag flags, int activationMode);

    void OnActionJump(int activationMode, float value);
private:
    Cry::DefaultComponents::CInputComponent* m_inputComponent{ nullptr };
	InputFlag m_inputFlags;
};
