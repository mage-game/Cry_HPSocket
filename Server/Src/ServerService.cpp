#include "stdafx.h"
#include "ServerService.h"

#define DEFAULT_ADDRESS _T("0.0.0.0")
#define DEFAULT_PORT 5555

ServerService* ServerService::m_ServerService = nullptr;

ServerService::ServerService()
{
	m_ServerService = this;
	// 创建监听器对象
	m_pListener = ::Create_HP_TcpPackServerListener();
	// 创建 Socket 对象
	m_pServer = ::Create_HP_TcpPackServer(m_pListener);
	// 设置 Socket 监听器回调函数
	::HP_Set_FN_Server_OnPrepareListen(m_pListener, OnPrepareListen);
	::HP_Set_FN_Server_OnAccept(m_pListener, OnAccept);
	::HP_Set_FN_Server_OnSend(m_pListener, OnSend);
	::HP_Set_FN_Server_OnReceive(m_pListener, OnReceive);
	::HP_Set_FN_Server_OnClose(m_pListener, OnClose);
	::HP_Set_FN_Server_OnShutdown(m_pListener, OnShutdown);

	m_DataHandlerPtr = std::make_shared<DataHandler>(m_pServer);
	m_DataHandlerPtr->SetDataFrameInterval(33);
}                         

ServerService::~ServerService()
{
	Stop();
	// 销毁 Socket 对象
	::Destroy_HP_TcpPackServer(m_pServer);
	// 销毁监听器对象
	::Destroy_HP_TcpServerListener(m_pListener);
}

void ServerService::Start()
{
	::HP_Server_SetSendPolicy(m_pServer, SP_DIRECT);
	::HP_Server_SetWorkerThreadCount(m_pServer, 10);
	::HP_Server_SetMaxConnectionCount(m_pServer, 1000);

	::HP_Server_SetFreeSocketObjPool(m_pServer, 500);
	::HP_Server_SetFreeSocketObjHold(m_pServer, 1500);
	::HP_Server_SetFreeBufferObjPool(m_pServer, 2000);
	::HP_Server_SetFreeBufferObjHold(m_pServer, 6000);
	::HP_TcpServer_SetSocketListenQueue(m_pServer, 2000);
	::HP_TcpServer_SetAcceptSocketCount(m_pServer, 2000);

	::HP_TcpPackServer_SetMaxPackSize(m_pServer, 0x01FFF);
	::HP_TcpPackServer_SetPackHeaderFlag(m_pServer, 0x169);
                                 
	if (::HP_Server_Start(m_pServer, DEFAULT_ADDRESS, DEFAULT_PORT))
	{
		LOG(INFO) << "$ Server Start OK";
	}
	else
	{
		LOG(ERROR) << "$ Server Start Fail" << ",code " + ::HP_Server_GetLastError(m_pServer);
	}
}

void ServerService::Stop()
{
	if (::HP_Server_Stop(m_pServer))
	{
		LOG(INFO) << "$ Server Stop";
	}
	else
	{
		LOG(ERROR) << "$ Server Stop Fail";
	}
}

En_HP_HandleResult __stdcall ServerService::OnPrepareListen(HP_Server pSender, SOCKET soListen)
{
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	::HP_Server_GetListenAddress(pSender, szAddress, &iAddressLen, &usPort);
	LOG(INFO) << "$ OnPrepareListen.bind address,ip:" << szAddress << " port:" << usPort;

	return HR_OK;
}

En_HP_HandleResult __stdcall ServerService::OnAccept(HP_Server pSender, HP_CONNID dwConnID, SOCKET soClient)
{
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;          

	if (!::HP_Server_GetRemoteAddress(pSender, dwConnID, szAddress, &iAddressLen, &usPort))
	{
		LOG(ERROR) << "> OnAccept.error,ConnID " << dwConnID;
		return HR_ERROR;
	}

	LOG(INFO) << "$ OnAccept.ConnID:"<< dwConnID << " ip:" << szAddress << ",port:" << usPort;

	return HR_OK;	
}

En_HP_HandleResult __stdcall ServerService::OnSend(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength)
{
	return HR_OK;
}

En_HP_HandleResult __stdcall ServerService::OnReceive(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength)
{
	m_ServerService->m_DataHandlerPtr->HandleData(dwConnID, (BYTE*)pData, iLength);

	return HR_OK;
}

En_HP_HandleResult __stdcall ServerService::OnClose(HP_Server pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode)
{
    if (enOperation== SO_CLOSE)
    {
        LOG(INFO) << "$ Client close,ConnID " << dwConnID;
    }
    else
    {
        LOG(ERROR) << "> OnClose.ConnID " << dwConnID << " error,SocketOperation " << enOperation << ",code " << iErrorCode;
    }

	return HR_OK;
}

En_HP_HandleResult __stdcall ServerService::OnShutdown(HP_Server pSender)
{
	LOG(INFO) << "$ OnShutdown";
	return HR_OK;
}

