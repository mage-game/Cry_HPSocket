#include "stdafx.h"
#include "MsgLogicDirectHandler.h"

MsgLogicDirectHandler::MsgLogicDirectHandler(HP_Object pSender)
	:MsgLogicHandlerBase(pSender)
{
}

MsgLogicDirectHandler::~MsgLogicDirectHandler()
{
}

void MsgLogicDirectHandler::SyncDataToAllClientDirectly(const uint8_t* pData, int iLength)
{
	std::vector<CONNID> connIDs = GetAllConnIDs();

	SendToClientByConnIDs(pData, iLength, connIDs);
}

void MsgLogicDirectHandler::SyncDataExceptConnIdDirectly(const uint8_t* pData, int iLength, CONNID connID)
{
	std::vector<CONNID> connIDs = GetPartConnIDs(connID);

	SendToClientByConnIDs(pData, iLength, connIDs);
}														 

void MsgLogicDirectHandler::SyncDataJustConnIdDirectly(const uint8_t* pData, int iLength, CONNID connID)
{
	std::vector<CONNID> connIDs;
	connIDs.push_back(connID);

	SendToClientByConnIDs(pData, iLength, connIDs);
}

void MsgLogicDirectHandler::GenerateAutoPkgBuffer(CBufferPtr& pkgBufferPtr, const PkgHeader& header, const uint8_t* data /*= nullptr*/)
{
	uint32_t headerLen = sizeof(PkgHeader);
	uint32_t bodyLen = header.bodyLen;

	pkgBufferPtr.Realloc(headerLen + bodyLen);

	memcpy(pkgBufferPtr.Ptr(), (uint8_t*)&header, headerLen);
	if (data != nullptr)
	{
		memcpy(pkgBufferPtr.Ptr() + headerLen, data, bodyLen);
	}
}

void MsgLogicDirectHandler::GenerateAllPlayerInfoPkg(CONNID connID, CBufferPtr& pkgBufferPtr, std::vector<CONNID> connIDs)
{			 	
	CBufferPtr bodyBufferPtr(connIDs.size() * sizeof(PkgBodyPlayers::Player));
	for (DWORD i = 0; i < connIDs.size(); i++)
	{
		PkgBodyPlayers::Player player;
		player.connID = connIDs[i];
		memcpy(bodyBufferPtr.Ptr() + i * sizeof(PkgBodyPlayers::Player), (uint8_t*)&player, sizeof(PkgBodyPlayers::Player));
	}

	PkgHeader header;
	header.connID = connID;
	header.bodyType = ALL_PLAYER_INFO;
	header.bodyLen = (uint32_t)bodyBufferPtr.Size();

	GenerateAutoPkgBuffer(pkgBufferPtr, header, bodyBufferPtr.Ptr());
}
