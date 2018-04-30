#pragma once
#include <CryExtension/ICryUnknown.h>
#include "IHPSocketClientListener.h"
#include "HPSocketSerializer.h"

namespace HPSocket
{
	class IHPSocketPlugin
	{
		CRYINTERFACE_DECLARE_GUID(IHPSocketPlugin, "E48F750D-E04A-4ffa-B827-AB300D4440BF"_cry_guid);

		virtual ~IHPSocketPlugin() {};
	public:
		virtual void RegisterSerializeListener(IClientListener* pClientListener, int type) = 0;
		virtual void RemoveSerializeListener(int type) = 0;
		virtual IClientListener* GetSerializableListener(int type) = 0;

		virtual bool SerializeSinglePkgDirectly(Serializer& serializer) = 0;
	};	 
}