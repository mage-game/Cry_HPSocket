#include "StdAfx.h"
#include "GameObjectCreatHelper.h"
#include "CryGame\IGameFramework.h"
#include "Components\SpawnPointComponent.h"
#include "Components\LocalPlayerComponent.h"
#include "Components\NetPlayerComponent.h"

GameObjectCreatHelper::GameObjectCreatHelper()
{

}

GameObjectCreatHelper::~GameObjectCreatHelper()
{

}

void GameObjectCreatHelper::SetLocalPlayerConnID(uint64_t connID)
{
	m_localPlayerConnID = connID;
}

const uint64_t GameObjectCreatHelper::GetLocalPlayerConnID() const
{
	return m_localPlayerConnID;
}

const uint64_t GameObjectCreatHelper::GetPlayerCount() const
{
	return m_playerMap.size();
}

IEntity* GameObjectCreatHelper::GetPlayerByConnID(uint64_t connID)
{
	return m_playerMap[connID];
}

IEntity* GameObjectCreatHelper::CreateLocalPlayer(const char* name)
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.sName = name;
	//spawnParams.id = LOCAL_PLAYER_ENTITY_ID;
	spawnParams.nFlags |= ENTITY_FLAG_CASTSHADOW;
			 	
	if (IEntity* pPlayerEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		if (auto pLocalPlayerComponent = pPlayerEntity->CreateComponent<LocalPlayerComponent>())
		{
			m_playerMap[m_localPlayerConnID] = pPlayerEntity; 
			return pPlayerEntity;
		}
	}
	return nullptr;
}	 

IEntity* GameObjectCreatHelper::CreateNetPlayer(const NetPlayerInfo *playerInfo)
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.sName = (char*)playerInfo->actor.name;
	spawnParams.nFlags |= ENTITY_FLAG_CASTSHADOW;

	if (IEntity* pPlayerEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		if (auto pLocalPlayerComponent = pPlayerEntity->CreateComponent<NetPlayerComponent>())
		{
			m_playerMap[playerInfo->connID] = pPlayerEntity;
			return pPlayerEntity;
		}
	}
	return nullptr;
}

bool GameObjectCreatHelper::SelectLocalActorTemplate(ActorInfo &actorInfo)
{
	if (IEntity* localPlayer = CreateLocalPlayer((char*)actorInfo.name))
	{
		if (SpawnLocalPlayer(localPlayer))
		{
			return true;
		}
	}
	return false;
}

bool GameObjectCreatHelper::SpawnLocalPlayer(IEntity* pPlayerEntity)
{
	// Spawn the player entity
	IEntityItPtr pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
	pEntityIterator->MoveFirst();

	while (!pEntityIterator->IsEnd())
	{
		IEntity *pSpawnPointEntity = pEntityIterator->Next();

		if (auto pSpawner = pSpawnPointEntity->GetComponent<CSpawnPointComponent>())
		{
			if (!pSpawner->IsSpawned())
			{
				pSpawner->SpawnEntity(pPlayerEntity);
				return true;
			}
		}
	}
	return false;
}
