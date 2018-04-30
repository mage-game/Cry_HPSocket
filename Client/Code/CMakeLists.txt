cmake_minimum_required (VERSION 3.6.2)
set(CRYENGINE_DIR "E:/Program Files (x86)/Crytek/CRYENGINE Launcher/Crytek/CRYENGINE_5.5")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "E:/Cryengine Projects/PingYaoVR/Client")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "Root"
		"GameConfiger.cpp"
		"GameObjectCreatHelper.cpp"
		"GamePlugin.cpp"
		"HPSocketPluginProxy.cpp"
		"StdAfx.cpp"
		"GameConfiger.h"
		"GameEnv.h"
		"GameObjectCreatHelper.h"
		"GamePlugin.h"
		"HPSocketPluginProxy.h"
		"PkgBody.h"
		"StdAfx.h"
)
add_sources("Animation_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "Animation"
)
add_sources("ProceduralClip_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "Animation\\\\ProceduralClip"
		"Animation/ProceduralClip/ProceduralClipAiming.cpp"
		"Animation/ProceduralClip/ProceduralClipLooking.cpp"
)
add_sources("ProceduralContext_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "Animation\\\\ProceduralContext"
		"Animation/ProceduralContext/ProceduralContextAim.cpp"
		"Animation/ProceduralContext/ProceduralContextLook.cpp"
		"Animation/ProceduralContext/ProceduralContextAim.h"
		"Animation/ProceduralContext/ProceduralContextHelpers.h"
		"Animation/ProceduralContext/ProceduralContextLook.h"
)
add_sources("Components_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "Components"
		"Components/LocalPlayerComponent.cpp"
		"Components/NetPlayerComponent.cpp"
		"Components/SpawnPointComponent.cpp"
		"Components/LocalPlayerComponent.h"
		"Components/NetPlayerComponent.h"
		"Components/SpawnPointComponent.h"
)
add_sources("PlayerComponents_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "Components\\\\PlayerComponents"
		"Components/PlayerComponents/PlayerAnimationComponent.cpp"
		"Components/PlayerComponents/PlayerInputComponent.cpp"
		"Components/PlayerComponents/PlayerMovementComponent.cpp"
		"Components/PlayerComponents/PlayerViewComponent.cpp"
		"Components/PlayerComponents/PlayerAnimationComponent.h"
		"Components/PlayerComponents/PlayerInputComponent.h"
		"Components/PlayerComponents/PlayerMovementComponent.h"
		"Components/PlayerComponents/PlayerViewComponent.h"
)
add_sources("FGNodes_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "FGNodes"
		"FGNodes/ClientNode.cpp"
)
add_sources("MsgHandlers_uber.cpp"
    PROJECTS PingYaoVR
    SOURCE_GROUP "MsgHandlers"
		"MsgHandlers/AllPlayerInfoHandler.cpp"
		"MsgHandlers/LocalPlayerLoginHandler.cpp"
		"MsgHandlers/NetPlayerLoginHandler.cpp"
		"MsgHandlers/PlayerAnimationHandler.cpp"
		"MsgHandlers/PlayerTransfromHandler.cpp"
)

end_sources()

CryEngineModule(PingYaoVR FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
		add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
		set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
		if (WIN32)
			set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/PingYaoVR.cryproject\"")
		endif()
	endif()
else()
	add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/PingYaoVR.cryproject\"")
	endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/PingYaoVR.cryproject\"")
    endif()

	add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/PingYaoVR.cryproject\"")
	endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/PingYaoVR.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.

set(HPSOCKET_PLUGIN_DIR "${PROJECT_DIR}/Plugins/HPSocketPlugin")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${HPSOCKET_PLUGIN_DIR}/Code/Common"
)

set_solution_startup_target(${THIS_PROJECT})

#END-CUSTOM