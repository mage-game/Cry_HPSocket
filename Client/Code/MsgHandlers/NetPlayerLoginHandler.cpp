#include "StdAfx.h"
#include <HPSocketPluginApi/IHPSocketClientLIstener.h>
#include <HPSocketPluginApi/HPSocketSerializer.h>
#include "PkgBody.h"

class NetPlayerLoginHandler :public HPSocket::IClientListener
{
public:
	NetPlayerLoginHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RegisterSerializeListener(this, NET_PLAYER_LOGIN);
	}

	~NetPlayerLoginHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RemoveSerializeListener(NET_PLAYER_LOGIN);
	}

	virtual void Unserialize(HPSocket::Serializer& unserializer) override
	{
		PkgBodyNetPlayerLogin* pBody = nullptr;
		uint64_t newNetPlayerConnID = unserializer.Unserialize(pBody); 

		mEnv->g_pGameObjectCreatHelper->CreateNetPlayer(&pBody->player);
	}
};

AUTO_REGISTER_HPSOCKET_HANDLER(NetPlayerLoginHandler)
