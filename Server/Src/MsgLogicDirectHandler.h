#pragma once
#include "MsgLogicHandlerBase.h"

class MsgLogicDirectHandler :public MsgLogicHandlerBase
{
public:
	explicit MsgLogicDirectHandler(HP_Object pSender);

	virtual ~MsgLogicDirectHandler();

	void GenerateAutoPkgBuffer(CBufferPtr& pkgBufferPtr, const PkgHeader& header, const uint8_t* data = nullptr);

	void GenerateAllPlayerInfoPkg(CONNID connID, CBufferPtr& pkgBufferPtr, std::vector<CONNID> connIDs);

	void SyncDataToAllClientDirectly(const uint8_t* pData, int iLength);

	void SyncDataExceptConnIdDirectly(const uint8_t* pData, int iLength, CONNID connID);
									   
	void SyncDataJustConnIdDirectly(const uint8_t* pData, int iLength, CONNID connID);
};
