#pragma once
#include <CryEntitySystem/IEntityComponent.h>

class CSpawnPointComponent final : public IEntityComponent
{
public:
	CSpawnPointComponent() = default;
	virtual ~CSpawnPointComponent() {}

	static void ReflectType(Schematyc::CTypeDesc<CSpawnPointComponent>& desc)
	{
		desc.SetGUID("{41316132-8A1E-4073-B0CD-A242FD3D2E90}"_cry_guid);
		desc.SetEditorCategory("Game");
		desc.SetLabel("SpawnPoint");
		desc.SetDescription("This spawn point can be used to spawn entities");
		desc.SetIcon("icons:ObjectTypes/object.ico");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Attach });
	}

	const bool IsSpawned() const;
public:
	void SpawnEntity(IEntity* otherEntity);
private:
	bool m_isSpawned = false;
};
