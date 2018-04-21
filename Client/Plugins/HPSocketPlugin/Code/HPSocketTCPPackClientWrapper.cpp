#include "StdAfx.h"
#include "HPSocketTCPPackClientWrapper.h" 

namespace HPSocket
{
	CTCPPackClientWrapper::CTCPPackClientWrapper()
	{
		m_TCPPackClientPtr = std::make_shared<TCPPackClientService>();
	}

	CTCPPackClientWrapper::~CTCPPackClientWrapper()
	{
		Stop();
	}

	const bool CTCPPackClientWrapper::IsStarted() const
    {
        return m_TCPPackClientPtr->IsStarted();
    }

	const bool CTCPPackClientWrapper::IsFinishHandShake() const
	{
		return m_TCPPackClientPtr->IsFinishHandShake();
	}

	bool CTCPPackClientWrapper::Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect)
    {
        return m_TCPPackClientPtr->Start(lpszRemoteAddress,usPort,bAsyncConnect);
    }

    bool CTCPPackClientWrapper::Stop()
    {
        return m_TCPPackClientPtr->Stop();
    }

    void CTCPPackClientWrapper::SetDataFrameInterval(float interval)
    {
        m_DataFrameInterval = interval;
    }

    void CTCPPackClientWrapper::SerializeSinglePkgDirectly(Serializer& serializer)
    {
		m_TCPPackClientPtr->SerializeSinglePkg(serializer);
    }

    void CTCPPackClientWrapper::SerializeAllPkgPeriodically(float frameTime)
    {
        m_CurrTime += frameTime*1000.0f;
        if (m_CurrTime > m_DataFrameInterval)
        {
            m_CurrTime = 0.0f;
            m_TCPPackClientPtr->SerializeAllPkgs();
        }
    }
}
