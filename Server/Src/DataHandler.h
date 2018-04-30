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

	void HandleData(uint64_t connID, uint8_t* data, uint32_t len);

	void SetDataFrameInterval(int interval= 33);  

private:       
	void RegisterAllActions();

private:
    std::shared_ptr<MsgHandlerAction> m_msgHandlerAction;
	std::shared_ptr<MsgLogicDirectHandler> m_msgLogicDirectHandler;
	std::shared_ptr<MsgLogicPeriodicalHandler> m_msgLogicPeriodicalHandler;
};

