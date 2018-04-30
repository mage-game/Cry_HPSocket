#pragma once

#include <unordered_map>
#include <functional>

class MsgHandlerAction
{
    typedef std::function<void(uint64_t connID, uint8_t* data, uint32_t len)> TActionCallback;
public:
    ~MsgHandlerAction();
    void RegisterAction(uint32_t type, TActionCallback callback);
    void ExecuteAction(uint32_t type, uint64_t connID, uint8_t* data, uint32_t len);
private:
    std::unordered_map<uint32_t, TActionCallback> m_actionCallbackMap;
};
