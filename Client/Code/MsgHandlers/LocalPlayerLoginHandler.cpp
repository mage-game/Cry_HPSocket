#include "StdAfx.h"
#include <HPSocketPluginApi/IHPSocketClientLIstener.h>
#include <HPSocketPluginApi/HPSocketSerializer.h>
#include "PkgBody.h"

class LocalPlayerLoginHandler :public HPSocket::IClientListener
{
public:
	LocalPlayerLoginHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RegisterSerializeListener(this, LOCAL_PLAYER_LOGIN);
	}
	~LocalPlayerLoginHandler()
	{
		mEnv->g_pHPSocketPluginProxy->RemoveSerializeListener(LOCAL_PLAYER_LOGIN);
	}

	virtual void Unserialize(HPSocket::Serializer& unserializer) override
	{
		uint64_t connID = unserializer.Unserialize();
		mEnv->g_pGameObjectCreatHelper->SetLocalPlayerConnID(connID);
	}

};

AUTO_REGISTER_HPSOCKET_HANDLER(LocalPlayerLoginHandler)  
