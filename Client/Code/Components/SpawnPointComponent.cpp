#include "StdAfx.h"
#include "SpawnPointComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>

void CSpawnPointComponent::SpawnEntity(IEntity* otherEntity)
{	   	
	auto spawnPointTM = GetEntity()->GetWorldTM();
	otherEntity->SetWorldTM(spawnPointTM);

	m_isSpawned = true;
}

const bool CSpawnPointComponent::IsSpawned() const
{
	return m_isSpawned;
}

AUTO_REGISTER_COMPONENT(CSpawnPointComponent)
