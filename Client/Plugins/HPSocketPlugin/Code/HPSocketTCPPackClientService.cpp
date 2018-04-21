#include "StdAfx.h"
#include "HPSocketPluginEnv.h"
#include "HPSocketTCPPackClientService.h"
#include "HPSocketPluginApi/IHPSocketPlugin.h"
#include "HPSocketPluginApi/PkgHeader.h"

namespace HPSocket
{
    TCPPackClientService* TCPPackClientService::m_TCPPackClientService = nullptr;

    TCPPackClientService::TCPPackClientService()
    {
        m_TCPPackClientService = this;
        m_pListener = ::Create_HP_TcpClientListener();
        m_pClient = ::Create_HP_TcpPackClient(m_pListener);

        ::HP_Set_FN_Client_OnConnect(m_pListener, OnConnect);
		::HP_Set_FN_Client_OnHandShake(m_pListener, OnHandShake);
        ::HP_Set_FN_Client_OnSend(m_pListener, OnSend);
        ::HP_Set_FN_Client_OnReceive(m_pListener, OnReceive);
        ::HP_Set_FN_Client_OnClose(m_pListener, OnClose);
    }

    TCPPackClientService::~TCPPackClientService()
    {
		Stop();

        ::Destroy_HP_TcpPackClient(m_pClient);
        ::Destroy_HP_TcpClientListener(m_pListener);
    }

    bool TCPPackClientService::Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect)
    {
        if (!m_Start)
        {
            ::HP_TcpPackServer_SetMaxPackSize(m_pClient, 0x01FFF);
            ::HP_TcpPackServer_SetPackHeaderFlag(m_pClient, 0x169);

            if (::HP_Client_Start(m_pClient, lpszRemoteAddress, usPort, bAsyncConnect))
            {
                m_Start = true;
				LogAlways("Start HP-Socket client Successed");

                return true;
            }
            else
            {
                m_Start = false;
				LogError("Start HP-Socket client Failed! error code %d,error desc %s",
                    ::HP_Client_GetLastError(m_pClient), HP_Client_GetLastErrorDesc(m_pClient));
            }
        }
        return false;
    }

    bool TCPPackClientService::Stop()
    {
        if (m_Start)
        {
            ::HP_Client_Stop(m_pClient);
            m_Start = false;
			m_FinishHandShake = false;
        }
        return true;
    }

    void TCPPackClientService::SerializeSinglePkg(Serializer& serializer)
    {
		if (m_Start)
		{
			BYTE* data = serializer.GetPkgBufferData();
			DWORD size = serializer.GetPkgBufferSize();
			Send(data, size);
		}
    }

    bool TCPPackClientService::Send(const BYTE* pData, DWORD size)
    {
        if (m_Start)
        {
            BOOL sendFlag = ::HP_Client_Send(m_pClient, pData, size);
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

    void TCPPackClientService::SerializeAllPkgs()
    {
        if (m_Start)
        {
			auto serializableListeners = mEnv->pSerializerContainer->GetAllSerializableListeners();
            for(auto& clientListenerPair : *serializableListeners)
            {
                Serializer serializer;
                if (IClientListener* pClientListener = clientListenerPair.second)
                {
                    pClientListener->Serialize(serializer);
                }
                SerializeSinglePkg(serializer);
            }
        }
    }

    const bool TCPPackClientService::IsStarted() const
    {
        return m_Start;
    }

	const bool TCPPackClientService::IsFinishHandShake() const
	{
		return m_FinishHandShake;
	}

	En_HP_HandleResult __stdcall TCPPackClientService::OnConnect(HP_Client pClient, HP_CONNID dwConnID)
    {
        TCHAR szAddress[40];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort;

        if (::HP_Client_GetLocalAddress(m_TCPPackClientService->m_pClient, szAddress, &iAddressLen, &usPort))
        {
            LogAlways("local address: %s#%d", szAddress, usPort);  
        }

        return HR_OK;
    }

	En_HP_HandleResult __stdcall TCPPackClientService::OnHandShake(HP_Client pSender, HP_CONNID dwConnID)
	{
		m_TCPPackClientService->m_FinishHandShake = true;
		return HR_OK;
	}

	En_HP_HandleResult __stdcall TCPPackClientService::OnSend(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pData, int iLength)
    {
        return HR_OK;
    }

    En_HP_HandleResult __stdcall TCPPackClientService::OnReceive(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pData, int iLength)
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
                if (IClientListener* pClientListener = mEnv->pSerializerContainer->GetUnserializableListener(bodyType))
                {
                    Serializer unserializer;
                    unserializer.SetPkgBufferData((BYTE*)header, pkgLen);
                    pClientListener->Unserialize(unserializer);
                }
            }

            currIndex += pkgLen;
        }

        return HR_OK;
    }

    En_HP_HandleResult __stdcall TCPPackClientService::OnClose(HP_Client pClient, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode)
    {
        return HR_OK;
    }
}
