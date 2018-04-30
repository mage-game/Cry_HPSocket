#include "StdAfx.h"
#include "HPSocketPluginProxy.h"
#include <CrySystem/ICryPluginManager.h>

namespace HPSocket
{		   
	HPSocketPluginProxy::HPSocketPluginProxy()
	{
		if (gEnv->IsDedicated()) return;

		if (Cry::IPluginManager* pPluginManager = gEnv->pSystem->GetIPluginManager())
		{
			if (m_HPSocketPlugin = pPluginManager->QueryPlugin<IHPSocketPlugin>())
			{
				Log("HPSocket plugin integration sucssefully loaded");
			}
			else
			{
				LogError("Error init HPSocket plugin integration - Can't get plugin!");
			}
		}
		else
		{
			LogError("Error init HPSocket plugin - Can't get factory!");
		}
	}

	HPSocketPluginProxy::~HPSocketPluginProxy()
	{

	}

	void HPSocketPluginProxy::RegisterSerializeListener(HPSocket::IClientListener* pClientListener, int type)
	{
		if (gEnv->IsDedicated()) return;

		if (m_HPSocketPlugin)
		{
			m_HPSocketPlugin->RegisterSerializeListener(pClientListener, type);
		}
	}

	void HPSocketPluginProxy::RemoveSerializeListener(int type)
	{
		if (gEnv->IsDedicated()) return;

		if (m_HPSocketPlugin)
		{
			m_HPSocketPlugin->RemoveSerializeListener(type);
		}
	}

	IClientListener* HPSocketPluginProxy::GetSerializableListener(int type)
	{
		if (gEnv->IsDedicated()) return nullptr;

		if (m_HPSocketPlugin)
		{
			return m_HPSocketPlugin->GetSerializableListener(type);
		}
		return nullptr;
	}

	bool HPSocketPluginProxy::SerializeSinglePkgDirectly(Serializer& serializer)
	{
		if (gEnv->IsDedicated()) return false;

		if (m_HPSocketPlugin)
		{
			return m_HPSocketPlugin->SerializeSinglePkgDirectly(serializer);
		}

		return false;
	}
}
