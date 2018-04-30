#pragma once

#include <CrySystem/ICryPlugin.h>
#include <CryEntitySystem/IEntityClass.h>

class CGamePlugin 
	: public Cry::IEnginePlugin
	, public ISystemEventListener
{
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CGamePlugin, "Game_Blank", "f01244b0-a4e7-4dc6-91e1-0ed18906fe7c"_cry_guid)

	PLUGIN_FLOWNODE_REGISTER
	PLUGIN_FLOWNODE_UNREGISTER

	virtual ~CGamePlugin();
	
	// ICryPlugin
	virtual const char* GetName() const override { return "GamePlugin"; }
	virtual const char* GetCategory() const override { return "Game"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~ICryPlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener 	
};