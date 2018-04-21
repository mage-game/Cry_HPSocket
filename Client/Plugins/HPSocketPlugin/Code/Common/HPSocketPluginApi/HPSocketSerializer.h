#pragma once
#include <stdint.h>
#include "bufferptr.h"
#include "PkgHeader.h"

namespace HPSocket
{
    class Serializer
    {
    public:
		void SetPkgBufferData(const uint8_t* pData, uint32_t len)
		{
			m_BufferPtr.Realloc(len);
			memcpy(m_BufferPtr.Ptr(), pData, len);
		}

		uint8_t* GetPkgBufferData()
		{
			return m_BufferPtr.Ptr();
		}

		size_t GetPkgBufferSize()
		{
			return m_BufferPtr.Size();
		}

		void Serialize(uint32_t bodyType)
		{
			PkgHeader header;
			header.bodyType = bodyType;
			header.bodyLen = 0;

			uint32_t headerLen = sizeof(PkgHeader);
			m_BufferPtr.Realloc(headerLen + header.bodyLen);

			memcpy(m_BufferPtr.Ptr(), (uint8_t*)&header, headerLen);
		}

		void Serialize(uint32_t bodyType, uint32_t bodyLen, const uint8_t* pBodyData)
		{
			PkgHeader header;
			header.bodyType = bodyType;
			header.bodyLen = bodyLen;

			uint32_t headerLen = sizeof(PkgHeader);
			m_BufferPtr.Realloc(headerLen + header.bodyLen);

			memcpy(m_BufferPtr.Ptr(), (uint8_t*)&header, headerLen);
			memcpy(m_BufferPtr.Ptr() + headerLen, pBodyData, header.bodyLen);
		}

        template <typename T>
		void Serialize(uint32_t bodyType, T& bodyData)
		{
			PkgHeader header;
			header.bodyType = bodyType;
			header.bodyLen = sizeof(T);

			uint32_t headerLen = sizeof(PkgHeader);
			m_BufferPtr.Realloc(headerLen + header.bodyLen);

			memcpy(m_BufferPtr.Ptr(), (uint8_t*)&header, headerLen);
			memcpy(m_BufferPtr.Ptr() + headerLen, (uint8_t*)&bodyData, header.bodyLen);
		}

		uint64_t Unserialize()
		{
			PkgHeader* pHeader = (PkgHeader*)m_BufferPtr.Ptr();
			return pHeader->connID;
		}

		uint64_t Unserialize(uint8_t*& pData, uint32_t& len)
		{
			PkgHeader* pHeader = (PkgHeader*)m_BufferPtr.Ptr();
			pData = pHeader->body;
			len = pHeader->bodyLen;
			return pHeader->connID;
		}

        template <typename T>
		uint64_t Unserialize(T*& pData)
		{
			PkgHeader* pHeader = (PkgHeader*)m_BufferPtr.Ptr();
			pData = (T*)pHeader->body;
			return pHeader->connID;
		}
    private:
        CBufferPtr m_BufferPtr;
    };
}
