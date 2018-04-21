#pragma once

#include <CryGame/IGameFramework.h>
#include "HPSocketTCPPackClientService.h" 

namespace HPSocket
{
	class CTCPPackClientWrapper
	{
	public:
		CTCPPackClientWrapper();
		virtual ~CTCPPackClientWrapper();
	public:
		const bool IsStarted() const;
		const bool IsFinishHandShake() const;

		bool Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect);
		bool Stop();

		void SetDataFrameInterval(float interval);

		void SerializeSinglePkgDirectly(Serializer& serializer);

		void SerializeAllPkgPeriodically(float frameTime);
	private:
		float m_CurrTime = 0.0f;
		float m_DataFrameInterval = 33.33f;
		std::shared_ptr<TCPPackClientService> m_TCPPackClientPtr;
	};
}
