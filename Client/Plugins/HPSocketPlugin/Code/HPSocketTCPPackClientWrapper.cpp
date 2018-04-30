#include "StdAfx.h"
#include "HPSocketTCPPackClientWrapper.h" 

namespace HPSocket
{
	TCPPackClientWrapper::TCPPackClientWrapper()
	{
		m_TCPPackClient = std::make_shared<TCPPackClientService>();
	}

	TCPPackClientWrapper::~TCPPackClientWrapper()
	{
		Stop();
	}

	const bool TCPPackClientWrapper::IsStarted() const
    {
        return m_TCPPackClient->IsStarted();
    }

	const bool TCPPackClientWrapper::IsFinishHandShake() const
	{
		return m_TCPPackClient->IsFinishHandShake();
	}

	bool TCPPackClientWrapper::Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect)
    {
        return m_TCPPackClient->Start(lpszRemoteAddress,usPort,bAsyncConnect);
    }

    bool TCPPackClientWrapper::Stop()
    {
        return m_TCPPackClient->Stop();
    }

    void TCPPackClientWrapper::SetDataFrameInterval(float interval)
    {
        m_dataFrameInterval = interval;
    }

    bool TCPPackClientWrapper::SerializeSinglePkgDirectly(Serializer& serializer)
    {
		return m_TCPPackClient->SerializeSinglePkg(serializer);
    }

    bool TCPPackClientWrapper::SerializeAllPkgPeriodically(float frameTime)
    {
        m_currTime += frameTime*1000.0f;
        if (m_currTime > m_dataFrameInterval)
        {
            m_currTime = 0.0f;
            return m_TCPPackClient->SerializeAllPkgs();
        }
		return false;
    }
}
