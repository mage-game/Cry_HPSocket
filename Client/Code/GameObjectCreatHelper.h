#pragma once

#include <unordered_map>
#include "PkgBody.h"

class GameObjectCreatHelper
{
public:
	GameObjectCreatHelper();
	~GameObjectCreatHelper();

	void SetLocalPlayerConnID(uint64_t connID);	
	const uint64_t GetLocalPlayerConnID() const;  	

	bool SelectLocalActorTemplate(ActorInfo &actorInfo);

	const uint64_t GetPlayerCount() const;	  
	IEntity* GetPlayerByConnID(uint64_t connID);   
	 
	IEntity* CreateNetPlayer(const NetPlayerInfo *playerInfo);
private:
	IEntity * CreateLocalPlayer(const char* name);
	bool SpawnLocalPlayer(IEntity* pPlayerEntity);

private:
	uint64_t m_localPlayerConnID = 0;

	std::unordered_map<uint64_t, IEntity*> m_playerMap;
};