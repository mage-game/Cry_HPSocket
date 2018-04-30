#pragma once

#include <vector>
#include <HPTypeDef.h>
#include <HPSocket4C.h>
#include "PkgHeader.h"
#include "PkgBody.h"

class MsgLogicHandlerBase
{
public:
	explicit MsgLogicHandlerBase(HP_Object pSender);
	virtual ~MsgLogicHandlerBase();	 

	std::vector<CONNID> GetAllConnIDs();

	std::vector<CONNID> GetPartConnIDs(CONNID connID);

	void SendToClientByConnIDs(const uint8_t* pData, int iLength, std::vector<CONNID>& connIDs);
protected:
	HP_TcpPackServer m_server;
};	


