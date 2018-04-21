#pragma once 

#include <HPSocket4C.h> 
#include "HPSocketPluginApi/HPSocketSerializer.h"

namespace HPSocket
{
    class TCPPackClientService 
    {
    public:      
        TCPPackClientService();
        ~TCPPackClientService();

        bool Start(LPCTSTR lpszRemoteAddress, USHORT usPort=5555, BOOL bAsyncConnect=TRUE);

        bool Stop();          
        
        void SerializeSinglePkg(Serializer& serializer);

        void SerializeAllPkgs();
 
        const bool IsStarted() const;   

		const bool IsFinishHandShake() const;
    private:
        bool Send(const BYTE* pData, DWORD size);
    private:
        static En_HP_HandleResult __stdcall OnConnect(HP_Client pClient, HP_CONNID dwConnID);
		static En_HP_HandleResult __stdcall OnHandShake(HP_Client pSender, HP_CONNID dwConnID);
        static En_HP_HandleResult __stdcall OnSend(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pData, int iLength);
        static En_HP_HandleResult __stdcall OnReceive(HP_Client pClient, HP_CONNID dwConnID, const BYTE* pData, int iLength);
        static En_HP_HandleResult __stdcall OnClose(HP_Client pClient, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);

    private:         
        static TCPPackClientService* m_TCPPackClientService;

        HP_TcpPackClient m_pClient;
        HP_TcpClientListener m_pListener;
                                                    
        bool m_Start = false;
		bool m_FinishHandShake = false;
    };
}

