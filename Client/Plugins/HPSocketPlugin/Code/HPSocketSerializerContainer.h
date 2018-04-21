#pragma once
                
#include <vector>
#include <unordered_map>
#include "HPSocketPluginApi\IHPSocketClientLIstener.h"

namespace HPSocket
{
    class SerializerContainer
    {
    public:
        ~SerializerContainer();

        void RegisterSerializeListener(IClientListener* pClientListener, int type);
        void RemoveSerializeListener(int type);   
        IClientListener* GetSerializableListener(int type);
        std::unordered_map<int, IClientListener*>* GetAllSerializableListeners();

        void RegisterUnserializeListener(IClientListener* pClientListener, int type);
        void RemoveUnserializeListener(int type);
        IClientListener* GetUnserializableListener(int type);
        std::unordered_map<int, IClientListener*>* GetAllUnserializableListeners();
      
    private:
        std::unordered_map<int, IClientListener*> m_SerializableListeners;
        std::unordered_map<int, IClientListener*> m_UnserializableListeners;
    };     
}
