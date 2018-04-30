#include "StdAfx.h"
#include <HPSocketPluginApi/IHPSocketClientLIstener.h>
#include <HPSocketPluginApi/HPSocketSerializer.h>
#include "PkgBody.h"
#include "Components\LocalPlayerComponent.h"
#include "Components\NetPlayerComponent.h"

class PlayerTransfromHandler :public HPSocket::IClientListener
{
public:
	PlayerTransfromHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RegisterSerializeListener(this, PLAYER_TRANSFROM);
	}

	~PlayerTransfromHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RemoveSerializeListener(PLAYER_TRANSFROM);
	}

	virtual void Serialize(HPSocket::Serializer& serializer) override
	{
		uint64_t connID = mEnv->g_pGameObjectCreatHelper->GetLocalPlayerConnID();
		if (auto pPlayer = mEnv->g_pGameObjectCreatHelper->GetPlayerByConnID(connID))
		{
			if (auto pPlayerComponent = pPlayer->GetComponent<LocalPlayerComponent>())
			{
				PkgBodyPlayerTransfrom pkgBody = pPlayerComponent->GetPlayerTransform();

				serializer.Serialize(PLAYER_TRANSFROM, pkgBody);
			}
		}
	}

	virtual void Unserialize(HPSocket::Serializer& unserializer) override
	{
		PkgBodyPlayerTransfrom* pkgBody = nullptr;
		uint64_t connID = unserializer.Unserialize(pkgBody);
		if (auto pPlayer = mEnv->g_pGameObjectCreatHelper->GetPlayerByConnID(connID))
		{
			if (auto pPlayerComponent = pPlayer->GetComponent<NetPlayerComponent>())
			{
				pPlayerComponent->SetPlayerTransform(*pkgBody);
			}
		}
	} 
};

AUTO_REGISTER_HPSOCKET_HANDLER(PlayerTransfromHandler)
