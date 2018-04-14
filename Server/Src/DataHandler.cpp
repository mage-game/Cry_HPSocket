#include "stdafx.h"
#include "DataHandler.h"

DataHandler::DataHandler(HP_Object pSender)
{
    m_MsgHandlerActionPtr = std::make_shared<MsgHandlerAction>();

	m_MsgLogicDirectHandlerPtr = std::make_shared<MsgLogicDirectHandler>(pSender);
	m_MsgLogicPeriodicalHandlerPtr = std::make_shared<MsgLogicPeriodicalHandler>(pSender);

	RegisterAllActions();

}

DataHandler::~DataHandler()
{
}	  
  
void DataHandler::SetDataFrameInterval(int interval/*= 33*/)
{
	m_MsgLogicPeriodicalHandlerPtr->SetDataFrameInterval(interval);
}		

void DataHandler::HandleData(CONNID dwConnID, BYTE* pData, uint32_t iLength)
{
    uint32_t currIndex = 0;
    while (currIndex < iLength)
    {
        PkgHeader* pHeader = (PkgHeader*)(pData + currIndex);
        pHeader->connID = dwConnID;
        currIndex += pHeader->bodyLen + sizeof(PkgHeader);

		LOG(INFO) << "$ Begin to handle msg,msg id is "<< pHeader->bodyType;
        m_MsgHandlerActionPtr->ExecuteAction(pHeader->bodyType, dwConnID, pData, iLength); 
		LOG(INFO) << "$ End to handle msg,msg id is " << pHeader->bodyType;
    }
}

void DataHandler::RegisterAllActions()
{
	m_MsgHandlerActionPtr->RegisterAction(PLAYER_LOGIN, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		m_MsgLogicDirectHandlerPtr->SyncDataJustConnIdDirectly(pData, iLength, dwConnID);
	});
	m_MsgHandlerActionPtr->RegisterAction(SELECT_ACTOR_INFO, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		m_MsgLogicDirectHandlerPtr->SyncDataExceptConnIdDirectly(pData, iLength, dwConnID);
	});
	m_MsgHandlerActionPtr->RegisterAction(ALL_PLAYER_INFO, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		CBufferPtr pkgBodyBufferPtr;
		std::vector<CONNID> connIDs = m_MsgLogicDirectHandlerPtr->GetPartConnIDs(dwConnID);
		m_MsgLogicDirectHandlerPtr->GenerateAllPlayerInfoPkg(dwConnID, pkgBodyBufferPtr, connIDs);

		m_MsgLogicDirectHandlerPtr->SyncDataJustConnIdDirectly(pkgBodyBufferPtr.Ptr(), (int)pkgBodyBufferPtr.Size(), dwConnID);
	});

	m_MsgHandlerActionPtr->RegisterAction(PLAYER_BIKE_TRANSFROM, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		m_MsgLogicPeriodicalHandlerPtr->SyncDataPeriodically(pData, iLength);
	});
	m_MsgHandlerActionPtr->RegisterAction(PLAYER_BIKE_ANIMATION, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		m_MsgLogicPeriodicalHandlerPtr->SyncDataPeriodically(pData, iLength);
	});
	m_MsgHandlerActionPtr->RegisterAction(PLAYER_TRANSFROM, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		m_MsgLogicPeriodicalHandlerPtr->SyncDataPeriodically(pData, iLength);
	});
	m_MsgHandlerActionPtr->RegisterAction(PLAYER_ANIMATION, [this](CONNID dwConnID, uint8_t* pData, uint32_t iLength)
	{
		m_MsgLogicPeriodicalHandlerPtr->SyncDataPeriodically(pData, iLength);
	});
}
