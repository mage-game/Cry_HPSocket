#include "StdAfx.h"
#include "PluginDll.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h> 
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

namespace HPSocket
{
	CPlugin_HP_Socket::~CPlugin_HP_Socket()
	{
		UnRegisterCVars();

		gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

		if (gEnv->pSchematyc)
		{
			gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CPlugin_HP_Socket::GetCID());
		}
	}

	bool CPlugin_HP_Socket::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
	{
		if (gEnv->IsDedicated())
		{
			return false;	 
		}

		gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CPlugin_VirZoom");	  		

		return true;
	}

	void CPlugin_HP_Socket::MainUpdate(float frameTime)
	{
		if (mEnv->pTCPPackClientWrapper == nullptr)
		{
			mEnv->pTCPPackClientWrapper->SerializeAllPkgPeriodically(frameTime);
		}
	}

	void CPlugin_HP_Socket::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
	{
		if (gEnv->IsDedicated())
		{
			return;
		}

		switch (event)
		{
		case ESYSTEM_EVENT_GAME_POST_INIT:
		{
			RegisterCVars();

			mEnv->Init();
		}
		break;
		case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
		{
			// Register all components that belong to this plug-in
			auto staticAutoRegisterLambda = [this](Schematyc::IEnvRegistrar& registrar)
			{
				// Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
				Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
			};

			if (gEnv->pSchematyc)
			{
				gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
					stl::make_unique<Schematyc::CEnvPackage>(
						CPlugin_HP_Socket::GetCID(),
						"EntityComponents",
						"Crytek GmbH",
						"Components",
						staticAutoRegisterLambda
						)
				);
			}
		}
		break;						
		case ESYSTEM_EVENT_LEVEL_UNLOAD:
		{
			mEnv->pTCPPackClientWrapper->Stop();
		}
		break;
		case ESYSTEM_EVENT_FULL_SHUTDOWN:
		case ESYSTEM_EVENT_FAST_SHUTDOWN:
		{
			mEnv->pTCPPackClientWrapper->Stop();
			EnableUpdate(IEnginePlugin::EUpdateStep::MainUpdate, false);
			UnRegisterCVars();
		}
		break;
		case ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED:
		{
			if (wparam == 0) // Clear when exit from game mode
			{
				mEnv->pTCPPackClientWrapper->Stop();
			}		  			
		}
		break;
		default: break;
		}
	}	

	void CPlugin_HP_Socket::RegisterSerializeListener(IClientListener* pClientListener, int type)
	{
		mEnv->pSerializerContainer->RegisterSerializeListener(pClientListener, type);
	}

	void CPlugin_HP_Socket::RemoveSerializeListener(int type)
	{
		mEnv->pSerializerContainer->RemoveSerializeListener(type);
	}

	IClientListener* CPlugin_HP_Socket::GetSerializableListener(int type)
	{
		return mEnv->pSerializerContainer->GetSerializableListener(type);
	}

	bool CPlugin_HP_Socket::SerializeSinglePkgDirectly(Serializer& serializer)
	{
		return mEnv->pTCPPackClientWrapper->SerializeSinglePkgDirectly(serializer);
	}

	void CPlugin_HP_Socket::RegisterCVars()
	{
		IConsole* pConsole = gEnv->pConsole;
		if (pConsole != nullptr && !gEnv->IsDedicated())
		{
			REGISTER_STRING("hp_socket_ip", "0.0.0.0", VF_NULL, "HPSocket ip");
			REGISTER_INT("hp_socket_port", 5555, VF_NULL, "HPSocket port");
			REGISTER_FLOAT("hp_socket_interval", 33.33f, VF_NULL, "HPSocket interval");

			REGISTER_CVAR2("hp_socke_debug_log", &mEnv->DebugLog, 0, VF_NULL, "HPSocket plugin debug log");
		}
	}

	void CPlugin_HP_Socket::UnRegisterCVars()
	{
		IConsole* pConsole = gEnv->pConsole;
		if (pConsole != nullptr && !gEnv->IsDedicated())
		{
			pConsole->UnregisterVariable("hp_socket_ip", true);
			pConsole->UnregisterVariable("hp_socket_port", true);
			pConsole->UnregisterVariable("hp_socket_interval", true);

			pConsole->UnregisterVariable("hp_socke_debug_log", true);
		}
	}

	CRYREGISTER_SINGLETON_CLASS(CPlugin_HP_Socket)
}
