#pragma once

#include <HPSocketPluginApi/IHPSocketPlugin.h>

namespace HPSocket
{
	//class IHPSocketPlugin;
	class IClientListener;
	class Serializer;

	class HPSocketPluginProxy :public IHPSocketPlugin
	{
	public:			
		HPSocketPluginProxy();
		~HPSocketPluginProxy();

		virtual void RegisterSerializeListener(IClientListener* pClientListener, int type) override;
		virtual void RemoveSerializeListener(int type) override;
		virtual IClientListener* GetSerializableListener(int type) override;
 
		virtual bool SerializeSinglePkgDirectly(Serializer& serializer) override;
	private:
		IHPSocketPlugin* m_HPSocketPlugin = nullptr;
	};
}


