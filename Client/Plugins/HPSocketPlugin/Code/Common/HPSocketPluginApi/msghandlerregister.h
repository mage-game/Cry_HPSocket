#pragma once

#include "IHPSocketClientListener.h"
#include "CryString\CryString.h"
#include <map>
#include <memory>

namespace HPSocket
{	   
	class MsgHandlerRegister
	{
	public:
		MsgHandlerRegister()
		{
			m_ClientListenerMap.clear();
		}
		~MsgHandlerRegister()
		{
			m_ClientListenerMap.clear();
		}
		template<typename T>
		bool Register()
		{
			if (std::is_base_of<HPSocket::IClientListener, T>::value)
			{
				string name = typeid(T).name();
				if (m_ClientListenerMap.find(name) == m_ClientListenerMap.end())
				{
					m_ClientListenerMap.insert(std::make_pair(name, std::make_shared<T>()));
					return true;
				}
			}
			return false;
		}
		template<typename T>
		T* GetMsgHandlerByType()
		{
			if (std::is_base_of<HPSocket::IClientListener, T>::value)
			{
				string name = typeid(T).name();
				if (m_ClientListenerMap.find(name) != m_ClientListenerMap.end())
				{
					auto pListerner= m_ClientListenerMap[name].get();
					return static_cast<T*>(pListerner);
				}
			}
			return nullptr;
		}
	private:
		std::map<string, std::shared_ptr<HPSocket::IClientListener> > m_ClientListenerMap;
	};
}