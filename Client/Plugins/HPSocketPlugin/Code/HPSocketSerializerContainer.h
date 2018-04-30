#pragma once
                
#include <unordered_map>
#include "HPSocketPluginApi\IHPSocketClientLIstener.h"

namespace HPSocket
{
    class SerializerContainer
    {
    public:
		SerializerContainer() = default;
        ~SerializerContainer();

        void RegisterSerializeListener(IClientListener* pClientListener, int type);
        void RemoveSerializeListener(int type);   
        IClientListener* GetSerializableListener(int type);
        std::unordered_map<int, IClientListener*>& GetAllSerializableListeners();
    private:
        std::unordered_map<int, IClientListener*> m_serializableListenerMap;
    };     
}
