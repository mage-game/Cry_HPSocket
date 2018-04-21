#pragma once
#include <CrySystem/ICryPlugin.h>
#include <CryEntitySystem/IEntityClass.h>
#include "HPSocketPluginApi/IHPSocketPlugin.h"

namespace HPSocket
{
	class CPlugin_HP_Socket
		: public Cry::IEnginePlugin
		, public IHPSocketPlugin
		, public ISystemEventListener
	{
	public:
		CRYINTERFACE_BEGIN()
		CRYINTERFACE_ADD(IHPSocketPlugin)
		//CRYINTERFACE_ADD(ISystemEventListener)
		CRYINTERFACE_ADD(Cry::IEnginePlugin)
		CRYINTERFACE_END()

		CRYGENERATE_SINGLETONCLASS_GUID(CPlugin_HP_Socket, "Plugin_HP_Socket", "19D74B74-CF54-4431-9BF4-215CF048CFF9"_cry_guid)

		PLUGIN_FLOWNODE_REGISTER
		PLUGIN_FLOWNODE_UNREGISTER

		virtual ~CPlugin_HP_Socket();
	public:
		//! Retrieve name of plugin.
		//virtual const char* GetName() const override { return "HP_Socket"; }
		//! Retrieve category for the plugin.
		//virtual const char* GetCategory() const override { return "Plugin"; }
		//! This is called to initialize the new plugin.
		virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;

		virtual void MainUpdate(float frameTime) override;

		// ISystemEventListener
		virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
		// ~ISystemEventListener

	public:
		// IHPSocketPlugin
		virtual void RegisterSerializeListener(IClientListener* pClientListener, int type) override;

		virtual void RemoveSerializeListener(int type) override;

		virtual IClientListener* GetSerializableListener(int type) override;

		virtual void RegisterUnserializeListener(IClientListener* pClientListener, int type) override;

		virtual void RemoveUnserializeListener(int type) override;

		virtual IClientListener* GetUnserializableListener(int type) override;

		virtual void SerializeSinglePkgDirectly(Serializer& serializer) override;
		// ~IHPSocketPlugin
	public:
		void RegisterCVars();
		void UnRegisterCVars();
	};
}
