#include "StdAfx.h"
#include "HPSocketPluginEnv.h"
#include "HPSocketTCPPackClientService.h"
#include "HPSocketPluginApi/IHPSocketPlugin.h"
#include "HPSocketPluginApi/PkgHeader.h"

namespace HPSocket
{
    TCPPackClientService* TCPPackClientService::s_TCPPackClientService = nullptr;

    TCPPackClientService::TCPPackClientService()
    {
        s_TCPPackClientService = this;
        m_listener = ::Create_HP_TcpClientListener();
        m_client = ::Create_HP_TcpPackClient(m_listener);

        ::HP_Set_FN_Client_OnConnect(m_listener, OnConnect);
		::HP_Set_FN_Client_OnHandShake(m_listener, OnHandShake);
        ::HP_Set_FN_Client_OnSend(m_listener, OnSend);
        ::HP_Set_FN_Client_OnReceive(m_listener, OnReceive);
        ::HP_Set_FN_Client_OnClose(m_listener, OnClose);
    }

    TCPPackClientService::~TCPPackClientService()
    {
		Stop();

        ::Destroy_HP_TcpPackClient(m_client);
        ::Destroy_HP_TcpClientListener(m_listener);
    }

    bool TCPPackClientService::Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect)
    {
        if (!m_isStarted)
        {
            ::HP_TcpPackServer_SetMaxPackSize(m_client, 0x01FFF);
            ::HP_TcpPackServer_SetPackHeaderFlag(m_client, 0x169);

            if (::HP_Client_Start(m_client, lpszRemoteAddress, usPort, bAsyncConnect))
            {
                m_isStarted = true;
				LogAlways("Start HP-Socket client Successed");

                return true;
            }
            else
            {
                m_isStarted = false;
				LogError("Start HP-Socket client Failed! error code %d,error desc %s",
                    ::HP_Client_GetLastError(m_client), HP_Client_GetLastErrorDesc(m_client));
            }
        }
        return false;
    }

    bool TCPPackClientService::Stop()
    {
        if (m_isStarted)
        {
            ::HP_Client_Stop(m_client);
            m_isStarted = false;
			m_isFinishHandShake = false;
        }
        return true;
    }

    bool TCPPackClientService::SerializeSinglePkg(Serializer& serializer)
    {
		if (m_isStarted)
		{
			BYTE* data = serializer.GetPkgBufferData();
			DWORD size = serializer.GetPkgBufferSize();
			return Send(data, size);
		}
		return false;
    }

    bool TCPPackClientService::Send(const BYTE* pData, DWORD size)
    {
        if (m_isStarted)
        {
            BOOL sendFlag = ::HP_Client_Send(m_client, pData, size);
            if (sendFlag)
            {
                LogDebug("HP-Socket client successed! length %d", size);
                return true;
            }
            else
            {
				LogError("error code %d,error desc %s",
                    ::SYS_GetLastError(), (char*)::HP_GetSocketErrorDesc(SE_DATA_SEND));
            }
        }
        return false;
    }

    bool TCPPackClientService::SerializeAllPkgs()
    {			
        if (m_isStarted)
        {
			bool isSendSucc = true;

			auto serializableListeners = mEnv->pSerializerContainer->GetAllSerializableListeners();
            for(auto& clientListenerPair : serializableListeners)
            {
                Serializer serializer;
                if (IClientListener* pClientListener = clientListenerPair.second)
                {
                    pClientListener->Serialize(serializer);

					if (!SerializeSinglePkg(serializer))
					{
						isSendSucc = false;
					}
                }		   				
            }
			return isSendSucc;
        }
		return false;
    }

	bool TCPPackClientService::UnserializeAllPkgs(const uint8_t* pData,const uint32_t iLength)
	{
		uint32_t currIndex = 0;
		while (currIndex < iLength)
		{
			PkgHeader* header = (PkgHeader*)(pData + currIndex);
			uint64_t connID = header->connID;
			uint32_t bodyType = header->bodyType;
			uint32_t bodyLen = header->bodyLen;
			uint32_t pkgLen = bodyLen + sizeof(PkgHeader);

			if (pkgLen <= (iLength - currIndex))
			{
				if (IClientListener* pClientListener = mEnv->pSerializerContainer->GetSerializableListener(bodyType))
				{
					Serializer unserializer;
					unserializer.SetPkgBufferData((uint8_t*)header, pkgLen);
					pClientListener->Unserialize(unserializer);
				}
			}

			currIndex += pkgLen;
		}

		return true;
	}

	const bool TCPPackClientService::IsStarted() const
    {
        return m_isStarted;
    }

	const bool TCPPackClientService::IsFinishHandShake() const
	{
		return m_isFinishHandShake;
	}

	En_HP_HandleResult __stdcall TCPPackClientService::OnConnect(HP_Client pClient, HP_CONNID dwConnID)
    {
        TCHAR szAddress[40];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort;

        if (::HP_Client_GetLocalAddress(s_TCPPackClientService->m_client, szAddress, &iAddressLen, &usPort))
        {
            LogAlways("local address: %s#%d", szAddress, usPort);  
        }

        return HR_OK;
    }

	En_HP_HandleResult __stdcall TCPPackClientService::OnHandShake(HP_Client pSender, HP_CONNID dwConnID)
	{
		s_TCPPackClientService->m_isFinishHandShake = true;
		return HR_OK;
	}

	En_HP_HandleResult __stdcall TCPPackClientService::OnSend(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pData, int iLength)
    {
        return HR_OK;
    }

    En_HP_HandleResult __stdcall TCPPackClientService::OnReceive(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pData, int iLength)
    {
		s_TCPPackClientService->UnserializeAllPkgs(pData, iLength);	  
        return HR_OK;
    }

    En_HP_HandleResult __stdcall TCPPackClientService::OnClose(HP_Client pClient, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode)
    {
        return HR_OK;
    }
}
