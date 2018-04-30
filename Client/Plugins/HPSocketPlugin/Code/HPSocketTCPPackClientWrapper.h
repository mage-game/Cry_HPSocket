#pragma once

#include "HPSocketTCPPackClientService.h" 

namespace HPSocket
{
	class TCPPackClientWrapper
	{
	public:
		TCPPackClientWrapper();
		virtual ~TCPPackClientWrapper();
	public:
		const bool IsStarted() const;
		const bool IsFinishHandShake() const;

		bool Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect);
		bool Stop();

		void SetDataFrameInterval(float interval);

		bool SerializeSinglePkgDirectly(Serializer& serializer);

		bool SerializeAllPkgPeriodically(float frameTime);
	private:
		float m_currTime = 0.0f;
		float m_dataFrameInterval = 33.33f;
		std::shared_ptr<TCPPackClientService> m_TCPPackClient;
	};
}
