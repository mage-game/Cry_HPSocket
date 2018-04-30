#include "stdafx.h"
#include "DataHandler.h"

DataHandler::DataHandler(HP_Object pSender)
{
	m_msgHandlerAction = std::make_shared<MsgHandlerAction>();

	m_msgLogicDirectHandler = std::make_shared<MsgLogicDirectHandler>(pSender);
	m_msgLogicPeriodicalHandler = std::make_shared<MsgLogicPeriodicalHandler>(pSender);

	RegisterAllActions();
}

DataHandler::~DataHandler()
{
}

void DataHandler::SetDataFrameInterval(int interval/*= 33*/)
{
	m_msgLogicPeriodicalHandler->SetDataFrameInterval(interval);
}

void DataHandler::HandleData(uint64_t connID, uint8_t* data, uint32_t len)
{
	uint32_t currIndex = 0;
	while (currIndex < len)
	{
		PkgHeader* header = (PkgHeader*)(data + currIndex);
		uint32_t pkgLen = header->bodyLen + sizeof(PkgHeader);
		currIndex += pkgLen;

		LOG(INFO) << "$ Begin to handle msg,msg id is " << header->bodyType;
		m_msgHandlerAction->ExecuteAction(header->bodyType, connID, (uint8_t*)header, pkgLen);
		LOG(INFO) << "$ End to handle msg,msg id is " << header->bodyType;
	}
}

void DataHandler::RegisterAllActions()
{
	m_msgHandlerAction->RegisterAction(LOCAL_PLAYER_LOGIN, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		CBufferPtr pkgBufferPtr;
		m_msgLogicDirectHandler->GenerateLocalPlayerLoginInfoPkg(connID, data, len, pkgBufferPtr);
		m_msgLogicDirectHandler->SyncDataJustConnIdDirectly(connID, pkgBufferPtr.Ptr(), (int)pkgBufferPtr.Size());
	});
	m_msgHandlerAction->RegisterAction(SELECT_ACTOR_INFO, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		PkgBodyNetPlayerLogin body = *(PkgBodyNetPlayerLogin*)(data + sizeof(PkgHeader));
		m_msgLogicDirectHandler->RecordPlayerLogin(connID, body.player);

		CBufferPtr pkgBufferPtr;
		m_msgLogicDirectHandler->GenerateNetPlayerLoginInfoPkg(connID, data, len, pkgBufferPtr);
		m_msgLogicDirectHandler->SyncDataExceptConnIdDirectly(connID, pkgBufferPtr.Ptr(), (int)pkgBufferPtr.Size());
	});
	m_msgHandlerAction->RegisterAction(ALL_NET_PLAYER_INFO, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		CBufferPtr pkgBufferPtr;
		m_msgLogicDirectHandler->GenerateAllPlayerInfoPkg(connID, data, len, pkgBufferPtr);
		m_msgLogicDirectHandler->SyncDataJustConnIdDirectly(connID, pkgBufferPtr.Ptr(), (int)pkgBufferPtr.Size());
	});

	m_msgHandlerAction->RegisterAction(PLAYER_BIKE_TRANSFROM, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		m_msgLogicPeriodicalHandler->SyncDataPeriodically(data, len);
	});
	m_msgHandlerAction->RegisterAction(PLAYER_BIKE_ANIMATION, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		m_msgLogicPeriodicalHandler->SyncDataPeriodically(data, len);
	});
	m_msgHandlerAction->RegisterAction(PLAYER_TRANSFROM, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		m_msgLogicPeriodicalHandler->SyncDataPeriodically(data, len);
	});
	m_msgHandlerAction->RegisterAction(PLAYER_ANIMATION, [this](uint64_t connID, uint8_t* data, uint32_t len)
	{
		m_msgLogicPeriodicalHandler->SyncDataPeriodically(data, len);
	});
}
