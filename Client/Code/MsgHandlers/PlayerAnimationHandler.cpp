#include "StdAfx.h"
#include <HPSocketPluginApi/IHPSocketClientLIstener.h>
#include <HPSocketPluginApi/HPSocketSerializer.h>
#include "PkgBody.h"
#include "Components\LocalPlayerComponent.h"
#include "Components\NetPlayerComponent.h"

class PlayerAnimationHandler :public HPSocket::IClientListener
{
public:
	PlayerAnimationHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RegisterSerializeListener(this, PLAYER_ANIMATION);
	}
	~PlayerAnimationHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RemoveSerializeListener(PLAYER_ANIMATION);
	}

	virtual void Serialize(HPSocket::Serializer& serializer) override
	{
		uint64_t connID = mEnv->g_pGameObjectCreatHelper->GetLocalPlayerConnID();
		if (auto pPlayer = mEnv->g_pGameObjectCreatHelper->GetPlayerByConnID(connID))
		{
			if (auto pPlayerComponent = pPlayer->GetComponent<LocalPlayerComponent>())
			{
				PkgBodyPlayerAnimation pkgBody = pPlayerComponent->GetPlayerAnimations();

				serializer.Serialize(PLAYER_ANIMATION, pkgBody);
			}
		}
	}

	virtual void Unserialize(HPSocket::Serializer& unserializer) override
	{
		PkgBodyPlayerAnimation* pkgBody = nullptr;
		uint64_t connID = unserializer.Unserialize(pkgBody);
		if (auto pPlayer = mEnv->g_pGameObjectCreatHelper->GetPlayerByConnID(connID))
		{
			if (auto pPlayerComponent = pPlayer->GetComponent<NetPlayerComponent>())
			{
				pPlayerComponent->SetPlayerAnimations(*pkgBody);
			}
		}
	}

};

AUTO_REGISTER_HPSOCKET_HANDLER(PlayerAnimationHandler)
