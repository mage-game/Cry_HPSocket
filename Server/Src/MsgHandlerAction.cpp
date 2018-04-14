#include "stdafx.h"
#include "MsgHandlerAction.h"

MsgHandlerAction::~MsgHandlerAction()
{
    m_ActionCallbackMap.clear();
}

void MsgHandlerAction::RegisterAction(uint32_t type, TActionCallback callback)
{
    m_ActionCallbackMap[type] = callback;
}

void MsgHandlerAction::ExecuteAction(uint32_t type, CONNID dwConnID, uint8_t* pData, uint32_t iLength)
{
    auto it = m_ActionCallbackMap.find(type);
    if (it != m_ActionCallbackMap.end())
    {
        auto func = it->second;
        func(dwConnID, pData, iLength);
    }
}
