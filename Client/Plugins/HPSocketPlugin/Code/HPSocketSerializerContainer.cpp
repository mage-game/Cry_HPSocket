// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "StdAfx.h"
#include "HPSocketSerializerContainer.h"   
#include "HPSocketPluginApi/IHPSocketClientLIstener.h"

namespace HPSocket
{
	SerializerContainer::~SerializerContainer()
    {
        m_SerializableListeners.clear();
        m_UnserializableListeners.clear();
    }

    void HPSocket::SerializerContainer::RegisterSerializeListener(IClientListener* pClientListener, int type)
    {
        auto it = m_SerializableListeners.find(type);
        if (it==m_SerializableListeners.end())
        {
            m_SerializableListeners[type] = pClientListener;
        }                
    }

    void HPSocket::SerializerContainer::RemoveSerializeListener(int type)
    {
        auto it = m_SerializableListeners.find(type);
        if (it != m_SerializableListeners.end())
        {
            m_SerializableListeners.erase(type);
        }
    }

    IClientListener* HPSocket::SerializerContainer::GetSerializableListener(int type)
    {
        return m_SerializableListeners[type];
    }

    std::unordered_map<int, IClientListener*>* HPSocket::SerializerContainer::GetAllSerializableListeners()
    {
        return &m_SerializableListeners;
    }


    void SerializerContainer::RegisterUnserializeListener(IClientListener* pClientListener, int type)
    {
        auto it = m_UnserializableListeners.find(type);
        if (it == m_UnserializableListeners.end())
        {
            m_UnserializableListeners[type] = pClientListener;
        }
    }

    void SerializerContainer::RemoveUnserializeListener(int type)
    {
        auto it = m_UnserializableListeners.find(type);
        if (it != m_UnserializableListeners.end())
        {
            m_UnserializableListeners.erase(type);
        }                                                   
    }

    IClientListener* SerializerContainer::GetUnserializableListener(int type)
    {
        return m_UnserializableListeners[type];
    }

    std::unordered_map<int, IClientListener*>* SerializerContainer::GetAllUnserializableListeners()
    {
        return &m_UnserializableListeners;
    }
}
