#include "StdAfx.h"
#include <HPSocketPluginApi/IHPSocketClientLIstener.h>
#include <HPSocketPluginApi/HPSocketSerializer.h>
#include "PkgBody.h"

class AllNetPlayerInfoHandler :public HPSocket::IClientListener
{
public:
	AllNetPlayerInfoHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RegisterSerializeListener(this, ALL_NET_PLAYER_INFO);
	}
	~AllNetPlayerInfoHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RemoveSerializeListener(ALL_NET_PLAYER_INFO);
	}

	virtual void Unserialize(HPSocket::Serializer& unserializer) override
	{
		uint8_t* pBody = nullptr;
		uint32_t size = 0;
		uint64_t localConnID = unserializer.Unserialize(pBody, size);
		uint64_t netPlayerCount = size / sizeof(NetPlayerInfo);
		for (int i = 0; i < netPlayerCount; i++)
		{
			NetPlayerInfo* playerInfo = (NetPlayerInfo*)pBody + i * sizeof(NetPlayerInfo);
			uint64_t connID = playerInfo->connID;
			if (mEnv->g_pGameObjectCreatHelper->GetPlayerByConnID(connID)!=nullptr)
			{
				mEnv->g_pGameObjectCreatHelper->CreateNetPlayer(playerInfo);
			} 			
		}
	} 
};

AUTO_REGISTER_HPSOCKET_HANDLER(AllNetPlayerInfoHandler)
