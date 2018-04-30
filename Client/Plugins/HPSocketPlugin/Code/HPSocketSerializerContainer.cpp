#include "StdAfx.h"
#include "HPSocketSerializerContainer.h"   

namespace HPSocket
{
	SerializerContainer::~SerializerContainer()
    {
        m_serializableListenerMap.clear();
    }

    void HPSocket::SerializerContainer::RegisterSerializeListener(IClientListener* pClientListener, int type)
    {
        auto it = m_serializableListenerMap.find(type);
        if (it==m_serializableListenerMap.end())
        {
            m_serializableListenerMap[type] = pClientListener;
        }                
    }

    void HPSocket::SerializerContainer::RemoveSerializeListener(int type)
    {
        auto it = m_serializableListenerMap.find(type);
        if (it != m_serializableListenerMap.end())
        {
            m_serializableListenerMap.erase(type);
        }
    }

    IClientListener* HPSocket::SerializerContainer::GetSerializableListener(int type)
    {
        return m_serializableListenerMap[type];
    }

    std::unordered_map<int, IClientListener*>& HPSocket::SerializerContainer::GetAllSerializableListeners()
    {
        return m_serializableListenerMap;
    }
}
