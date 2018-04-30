#include "stdafx.h"
#include "MsgHandlerAction.h"

MsgHandlerAction::~MsgHandlerAction()
{
    m_actionCallbackMap.clear();
}

void MsgHandlerAction::RegisterAction(uint32_t type, TActionCallback callback)
{
    m_actionCallbackMap[type] = callback;
}

void MsgHandlerAction::ExecuteAction(uint32_t type, uint64_t connID, uint8_t* data, uint32_t len)
{
    auto it = m_actionCallbackMap.find(type);
    if (it != m_actionCallbackMap.end())
    {
        auto func = it->second;
        func(connID, data, len);
    }
}
