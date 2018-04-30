#pragma once
#include "MsgLogicHandlerBase.h"
#include "DataPool.h"

class MsgLogicPeriodicalHandler :public MsgLogicHandlerBase
{
public:
	explicit MsgLogicPeriodicalHandler(HP_Object pSender);

	virtual ~MsgLogicPeriodicalHandler();

	void SetDataFrameInterval(int interval = 33);

	void SyncDataPeriodically(const BYTE* pData, int iLength);

	void PutDataToBuffer(const BYTE* pData, int iLength);

	void SendBufferData();

	static uint32_t WINAPI WorkerThreadProc(LPVOID pv);
private:
	int m_dataFrameInterval = 33;
	bool m_dataHandleThreadStart = true;

	HANDLE m_hThread;

	CCriSec m_CS;
	std::shared_ptr<DataPool> m_dataPoolPtr;
};
