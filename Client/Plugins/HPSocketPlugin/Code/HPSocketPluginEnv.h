#pragma once

#include "HPSocketSerializerContainer.h"
#include "HPSocketTCPPackClientWrapper.h"

#define TITLE "[HPSocketPlugin] "

#define Log(...) do {  CryLog (TITLE __VA_ARGS__); } while(0)
#define LogAlways(...) do { CryLogAlways (TITLE __VA_ARGS__); } while(0)
#define LogWarning(...) do { CryWarning(VALIDATOR_MODULE_RENDERER, VALIDATOR_WARNING, TITLE __VA_ARGS__); } while(0)
#define LogError(...) do { CryWarning(VALIDATOR_MODULE_RENDERER, VALIDATOR_ERROR, TITLE __VA_ARGS__); } while(0)

namespace HPSocket
{
	// Global plugin environment
	struct SPluginEnv
	{
		void Init()
		{
			pSerializerContainer = std::make_shared<SerializerContainer>();
			pTCPPackClientWrapper = std::make_shared<TCPPackClientWrapper>();
		}
		// Pointers
		std::shared_ptr<SerializerContainer> pSerializerContainer;
		std::shared_ptr<TCPPackClientWrapper> pTCPPackClientWrapper;

		// Console variables	
		int DebugLog = 0;
	};

	extern std::shared_ptr<SPluginEnv> mEnv;
}

#define LogDebug(...) do {  if(mEnv->DebugLog) {CryLog (TITLE __VA_ARGS__);} } while(0)

