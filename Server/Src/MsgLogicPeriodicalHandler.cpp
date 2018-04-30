#include "stdafx.h"
#include "MsgLogicPeriodicalHandler.h"

MsgLogicPeriodicalHandler::MsgLogicPeriodicalHandler(HP_Object pSender)
	:MsgLogicHandlerBase(pSender)
{
	m_dataPoolPtr = std::make_shared<DataPool>();

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThreadProc, (LPVOID)this, 0, nullptr);
}

MsgLogicPeriodicalHandler::~MsgLogicPeriodicalHandler()
{
	::CloseHandle(m_hThread);
}

void MsgLogicPeriodicalHandler::SetDataFrameInterval(int interval /*= 33*/)
{
	m_dataFrameInterval = interval;
}

void MsgLogicPeriodicalHandler::SyncDataPeriodically(const BYTE* pData, int iLength)
{
	PutDataToBuffer(pData, iLength);
}

void MsgLogicPeriodicalHandler::PutDataToBuffer(const BYTE* pData, int iLength)
{
	CCriSecLock locallock(m_CS);

	m_dataPoolPtr->PutData(pData, iLength);
}

void MsgLogicPeriodicalHandler::SendBufferData()
{
	CCriSecLock locallock(m_CS);

	CBufferPtr dataBufferPtr;
	m_dataPoolPtr->GetAllData(dataBufferPtr);
	if (dataBufferPtr.Size() > 0)
	{
		std::vector<CONNID> connIDs = GetAllConnIDs();
		SendToClientByConnIDs(dataBufferPtr.Ptr(), (int)dataBufferPtr.Size(), connIDs);
	}
}

uint32_t WINAPI MsgLogicPeriodicalHandler::WorkerThreadProc(LPVOID pv)
{
	MsgLogicPeriodicalHandler* handler = (MsgLogicPeriodicalHandler*)pv;

	LOG(INFO) << "$ Start to SyncData,the dataframe Interval is " << handler->m_dataFrameInterval;

	while (handler->m_dataHandleThreadStart)
	{
		handler->SendBufferData();

		Sleep(handler->m_dataFrameInterval);
	}
	return 0;
}
