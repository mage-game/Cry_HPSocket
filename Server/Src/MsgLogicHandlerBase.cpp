#include "stdafx.h"
#include "MsgLogicHandlerBase.h"
		  
MsgLogicHandlerBase::MsgLogicHandlerBase(HP_Object pSender)
	:m_server(pSender)
{		   
}

MsgLogicHandlerBase::~MsgLogicHandlerBase()
{
}

std::vector<CONNID> MsgLogicHandlerBase::GetAllConnIDs()
{
	DWORD connCount = 0;
	std::vector<CONNID> connIDs;
	do
	{
		connCount = ::HP_Server_GetConnectionCount(m_server);
		if (connCount == 0)
		{
			break;
		}
		connIDs.resize(connCount);
		if (::HP_Server_GetAllConnectionIDs(m_server, &connIDs[0], &connCount))
		{
			break;
		}
	} while (true);

	return connIDs;
}

std::vector<CONNID> MsgLogicHandlerBase::GetPartConnIDs(CONNID connID)
{
	std::vector<CONNID> connIDs = GetAllConnIDs();
	auto it = std::find(connIDs.begin(), connIDs.end(), connID);
	if (it != connIDs.end())
	{
		connIDs.erase(it);
	}
	return connIDs;
}

void MsgLogicHandlerBase::SendToClientByConnIDs(const uint8_t* pData, int iLength, std::vector<CONNID>& connIDs)
{
	for (auto connID : connIDs)
	{
		if (!::HP_Server_Send(m_server, connID, pData, iLength))
		{
			LOG(ERROR) << "> Send faild, ConnID " << connID;
		}
		//static uint32_t m_FrameId = 0;;
		//uint32_t frameId = ++m_FrameId;
		//LOG(DEBUG) << "> SendToAllClient.The frame id is " << frameId << ",the data length is " << iLength;
	}
}
