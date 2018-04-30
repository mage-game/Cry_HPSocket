#pragma once
#include "HPSocketPluginProxy.h"
#include <CrySchematyc/Env/IEnvRegistry.h>
#include "GameConfiger.h"
#include "GameObjectCreatHelper.h"

#define TITLE "[Game] "

#define Log(...) do {  CryLog (TITLE __VA_ARGS__); } while(0)
#define LogAlways(...) do { CryLogAlways (TITLE __VA_ARGS__); } while(0)
#define LogWarning(...) do { CryWarning(VALIDATOR_MODULE_RENDERER, VALIDATOR_WARNING, TITLE __VA_ARGS__); } while(0)
#define LogError(...) do { CryWarning(VALIDATOR_MODULE_RENDERER, VALIDATOR_ERROR, TITLE __VA_ARGS__); } while(0)

// Global plugin environment
struct SPluginEnv
{			  	
	SPluginEnv()
	{	
		g_pGameObjectCreatHelper = std::make_shared<GameObjectCreatHelper>();
		g_pGameConfiger = std::make_shared<GameConfiger>();
	}
	void Init()
	{
		g_pHPSocketPluginProxy = std::make_shared<HPSocket::HPSocketPluginProxy>();
	}

	std::shared_ptr<GameObjectCreatHelper> g_pGameObjectCreatHelper;
	std::shared_ptr<GameConfiger> g_pGameConfiger;

	std::shared_ptr<HPSocket::HPSocketPluginProxy> g_pHPSocketPluginProxy;	

	int DebugLog = 0;
};

extern std::shared_ptr<SPluginEnv> mEnv;

#define LogDebug(...) do {  if(mEnv->DebugLog) {CryLog (TITLE __VA_ARGS__);} } while(0)

#define AUTO_REGISTER_HPSOCKET_HANDLER(classname) \
static void Register##classname(Schematyc::IEnvRegistrar& registrar)\
{\
	string name = typeid(classname).name();\
	if (std::is_base_of<HPSocket::IClientListener, classname>::value)\
	{\
		static classname classname##Obj; \
		LogAlways("Register msg handler %s successed!",name);\
	}\
	else\
	{\
		LogError("Register msg handler %s failed!",name);\
	}\
}\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register##classname)

#define AUTO_REGISTER_COMPONENT(classname)\
static void Register##classname(Schematyc::IEnvRegistrar& registrar)\
{  \
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID()); \
	{ \
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(classname));\
	}\
}\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register##classname)
