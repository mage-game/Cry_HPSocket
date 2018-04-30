#include "stdafx.h"
#include "MsgLogicDirectHandler.h"

MsgLogicDirectHandler::MsgLogicDirectHandler(HP_Object pSender)
	:MsgLogicHandlerBase(pSender)
{
}

MsgLogicDirectHandler::~MsgLogicDirectHandler()
{
}

void MsgLogicDirectHandler::RecordPlayerLogin(uint64_t connID, NetPlayerInfo &player)
{
	m_playerMap[connID] = player;
}

void MsgLogicDirectHandler::SyncDataToAllClientDirectly(const uint8_t* pData, int iLength)
{
	std::vector<uint64_t> connIDs = GetAllConnIDs();

	SendToClientByConnIDs(pData, iLength, connIDs);
}

void MsgLogicDirectHandler::SyncDataExceptConnIdDirectly(uint64_t connID, const uint8_t* pData, int iLength)
{
	std::vector<uint64_t> connIDs = GetPartConnIDs(connID);

	SendToClientByConnIDs(pData, iLength, connIDs);
}

void MsgLogicDirectHandler::SyncDataJustConnIdDirectly(uint64_t connID, const uint8_t* pData, int iLength)
{
	std::vector<uint64_t> connIDs;
	connIDs.push_back(connID);

	SendToClientByConnIDs(pData, iLength, connIDs);
}

bool MsgLogicDirectHandler::GenerateLocalPlayerLoginInfoPkg(uint64_t connID, uint8_t* data, uint32_t len, CBufferPtr& pkgBufferPtr)
{
	PkgHeader header;
	header.connID = connID;
	header.bodyType = LOCAL_PLAYER_LOGIN;
	header.bodyLen = 0;

	//PkgBodyLocalPlayerLogin body;

	pkgBufferPtr.Cat((uint8_t*)&header, sizeof(PkgHeader));
	//pkgBufferPtr.Cat((uint8_t*)&body, sizeof(PkgBodyLocalPlayerLogin));

	return true;
}

bool MsgLogicDirectHandler::GenerateNetPlayerLoginInfoPkg(uint64_t connID, uint8_t* data, uint32_t len, CBufferPtr& pkgBufferPtr)
{
	auto it = m_playerMap.find(connID);
	if (it != m_playerMap.end())
	{
		PkgHeader header;
		header.connID = connID;
		header.bodyType = NET_PLAYER_LOGIN;
		header.bodyLen = sizeof(PkgBodyNetPlayerLogin);

		PkgBodyNetPlayerLogin body;
		body.player = it->second;

		pkgBufferPtr.Cat((uint8_t*)&header, sizeof(PkgHeader));
		pkgBufferPtr.Cat((uint8_t*)&body, sizeof(PkgBodyNetPlayerLogin));

		return true;
	}
	return false;
}					

bool MsgLogicDirectHandler::GenerateAllPlayerInfoPkg(uint64_t connID, uint8_t* data, uint32_t len, CBufferPtr& pkgBufferPtr)
{
	CBufferPtr bodyBufferPtr;
	std::vector<CONNID> connIDs = GetPartConnIDs(connID);
	for (auto id : connIDs)
	{
		auto it = m_playerMap.find(id);
		if (it != m_playerMap.end())
		{
			NetPlayerInfo player = it->second;
			bodyBufferPtr.Cat((uint8_t*)&player, sizeof(NetPlayerInfo));
		}
	}

	PkgHeader header;
	header.connID = connID;
	header.bodyType = ALL_NET_PLAYER_INFO;
	header.bodyLen = (uint32_t)bodyBufferPtr.Size();

	pkgBufferPtr.Cat((uint8_t*)&header, sizeof(PkgHeader));
	pkgBufferPtr.Cat(bodyBufferPtr);

	return true;
}
