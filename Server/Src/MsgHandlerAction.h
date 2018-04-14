#pragma once

#include <unordered_map>
#include <functional>
#include <HPTypeDef.h>

class MsgHandlerAction
{
    typedef std::function<void(CONNID dwConnID, uint8_t* pData, uint32_t iLength)> TActionCallback;
public:
    ~MsgHandlerAction();
    void RegisterAction(uint32_t type, TActionCallback callback);
    void ExecuteAction(uint32_t type, CONNID dwConnID, uint8_t* pData, uint32_t iLength);
private:
    std::unordered_map<uint32_t, TActionCallback> m_ActionCallbackMap;
};
