#pragma once
#include <unordered_map>
#include "MsgLogicHandlerBase.h"

class MsgLogicDirectHandler :public MsgLogicHandlerBase
{
public:
	explicit MsgLogicDirectHandler(HP_Object pSender);

	virtual ~MsgLogicDirectHandler();

	void RecordPlayerLogin(uint64_t connID, NetPlayerInfo &player);

	void SyncDataToAllClientDirectly(const uint8_t* pData, int iLength);

	void SyncDataExceptConnIdDirectly(uint64_t connID, const uint8_t* pData, int iLength);

	void SyncDataJustConnIdDirectly(uint64_t connID, const uint8_t* pData, int iLength);

	bool GenerateLocalPlayerLoginInfoPkg(uint64_t connID, uint8_t* data, uint32_t len, CBufferPtr& pkgBufferPtr);

	bool GenerateNetPlayerLoginInfoPkg(uint64_t connID, uint8_t* data, uint32_t len, CBufferPtr& pkgBufferPtr);

	bool GenerateAllPlayerInfoPkg(uint64_t connID, uint8_t* data, uint32_t len, CBufferPtr& pkgBufferPtr);

private:
	std::unordered_map<uint64_t, NetPlayerInfo> m_playerMap;
};
