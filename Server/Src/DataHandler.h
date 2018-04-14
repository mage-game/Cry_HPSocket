#pragma once
  
#include "HPSocket4C.h"
#include "PkgHeader.h"
#include "MsgHandlerAction.h"
#include "MsgLogicDirectHandler.h"
#include "MsgLogicPeriodicalHandler.h"
                             
class DataHandler
{
public:
    explicit DataHandler(HP_Object pSender);	

	~DataHandler();

	void HandleData(CONNID dwConnID, BYTE* pData, uint32_t iLength);

	void SetDataFrameInterval(int interval= 33);  

private:       
	void RegisterAllActions();

private:
    std::shared_ptr<MsgHandlerAction> m_MsgHandlerActionPtr;
	std::shared_ptr<MsgLogicDirectHandler> m_MsgLogicDirectHandlerPtr;
	std::shared_ptr<MsgLogicPeriodicalHandler> m_MsgLogicPeriodicalHandlerPtr;
};

