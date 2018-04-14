#pragma once

#include "DataHandler.h"

class ServerService
{
public:
	ServerService();
	~ServerService();

	void Start();
	void Stop();

private:
	static En_HP_HandleResult __stdcall OnPrepareListen(HP_Server pSender, SOCKET soListen);
	static En_HP_HandleResult __stdcall OnAccept(HP_Server pSender, HP_CONNID dwConnID, SOCKET soClient);
    static En_HP_HandleResult __stdcall OnSend(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
	static En_HP_HandleResult __stdcall OnReceive(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
	static En_HP_HandleResult __stdcall OnClose(HP_Server pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);
	static En_HP_HandleResult __stdcall OnShutdown(HP_Server pSender);
private:
	static ServerService* m_ServerService;

	HP_TcpPackServer m_pServer;
	HP_TcpServerListener m_pListener;

	std::shared_ptr<DataHandler> m_DataHandlerPtr;
};

