cmake_minimum_required (VERSION 3.6.2)
set(CRYENGINE_DIR "E:/Program Files (x86)/Crytek/CRYENGINE Launcher/Crytek/CRYENGINE_5.5")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "E:/Cryengine Projects/PingYaoVR/Client/Plugins/HPSocketPlugin")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS HPSocketPlugin
    SOURCE_GROUP "Root"
		"HPSocketSerializerContainer.cpp"
		"HPSocketTCPPackClientNode.cpp"
		"HPSocketTCPPackClientService.cpp"
		"HPSocketTCPPackClientWrapper.cpp"
		"HPSocketXmlHelper.cpp"
		"PluginDll.cpp"
		"StdAfx.cpp"
		"HPSocketPluginEnv.h"
		"HPSocketSerializerContainer.h"
		"HPSocketTCPPackClientService.h"
		"HPSocketTCPPackClientWrapper.h"
		"HPSocketXmlHelper.h"
		"PluginDll.h"
		"StdAfx.h"
)
add_sources("Common_uber.cpp"
    PROJECTS HPSocketPlugin
    SOURCE_GROUP "Common"
)
add_sources("HPSocketPluginApi_uber.cpp"
    PROJECTS HPSocketPlugin
    SOURCE_GROUP "Common\\\\HPSocketPluginApi"
		"Common/HPSocketPluginApi/BufferPtr.h"
		"Common/HPSocketPluginApi/HPSocketSerializer.h"
		"Common/HPSocketPluginApi/MsgHandlerRegister.h"
		"Common/HPSocketPluginApi/IHPSocketClientLIstener.h"
		"Common/HPSocketPluginApi/IHPSocketPlugin.h"
		"Common/HPSocketPluginApi/PkgHeader.h"
)

end_sources()

CryEngineModule(HPSocketPlugin FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

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
			set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/Plugins/HPSocketPlugin/HPSocketPlugin.cryproject\"")
		endif()
	endif()
else()
	add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/Plugins/HPSocketPlugin/HPSocketPlugin.cryproject\"")
	endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/Plugins/HPSocketPlugin/HPSocketPlugin.cryproject\"")
    endif()

	add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
	set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
	if (WIN32)
		set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/Plugins/HPSocketPlugin/HPSocketPlugin.cryproject\"")
	endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"E:/Cryengine Projects/PingYaoVR/Client/Plugins/HPSocketPlugin/HPSocketPlugin.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.

set(3RD_HPSOCKET_DIR "${PROJECT_DIR}/3rds/HP-Socket-5.2.1/Windows")

target_include_directories(${THIS_PROJECT}
PRIVATE
	"${PROJECT_DIR}/Code/Common"
	"${3RD_HPSOCKET_DIR}/Common/Src"
	"${3RD_HPSOCKET_DIR}/Src"
)

target_link_libraries( ${THIS_PROJECT} 
	PRIVATE 
		"${3RD_HPSOCKET_DIR}/Bin/HPSocket4C/x64/HPSocket4C_D.lib" 
	)
	
set_solution_startup_target(${THIS_PROJECT})

#END-CUSTOM