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
			m_buffer.Realloc(len);
			memcpy(m_buffer.Ptr(), pData, len);
		}

		uint8_t* GetPkgBufferData()
		{
			return m_buffer.Ptr();
		}

		size_t GetPkgBufferSize()
		{
			return m_buffer.Size();
		}

		void Serialize(uint32_t bodyType)
		{
			PkgHeader header;
			header.bodyType = bodyType;
			header.bodyLen = 0;

			uint32_t headerLen = sizeof(PkgHeader);
			m_buffer.Realloc(headerLen + header.bodyLen);

			memcpy(m_buffer.Ptr(), (uint8_t*)&header, headerLen);
		}

		void Serialize(uint32_t bodyType, uint32_t bodyLen, const uint8_t* pBodyData)
		{
			PkgHeader header;
			header.bodyType = bodyType;
			header.bodyLen = bodyLen;

			uint32_t headerLen = sizeof(PkgHeader);
			m_buffer.Realloc(headerLen + header.bodyLen);

			memcpy(m_buffer.Ptr(), (uint8_t*)&header, headerLen);
			memcpy(m_buffer.Ptr() + headerLen, pBodyData, header.bodyLen);
		}

        template <typename T>
		void Serialize(uint32_t bodyType, T& bodyData)
		{
			PkgHeader header;
			header.bodyType = bodyType;
			header.bodyLen = sizeof(T);

			uint32_t headerLen = sizeof(PkgHeader);
			m_buffer.Realloc(headerLen + header.bodyLen);

			memcpy(m_buffer.Ptr(), (uint8_t*)&header, headerLen);
			memcpy(m_buffer.Ptr() + headerLen, (uint8_t*)&bodyData, header.bodyLen);
		}

		uint64_t Unserialize()
		{
			PkgHeader* header = (PkgHeader*)m_buffer.Ptr();
			return header->connID;
		}

		uint64_t Unserialize(uint8_t*& pData, uint32_t& len)
		{
			PkgHeader* header = (PkgHeader*)m_buffer.Ptr();
			pData = header->body;
			len = header->bodyLen;
			return header->connID;
		}

        template <typename T>
		uint64_t Unserialize(T*& pData)
		{
			PkgHeader* header = (PkgHeader*)m_buffer.Ptr();
			pData = (T*)header->body;
			return header->connID;
		}
    private:
        CBufferPtr m_buffer;
    };
}
