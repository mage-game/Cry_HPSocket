#include "stdafx.h"
#include "DataPool.h"

#define MAX_BLOCK_SIZE (64*1024)
#define DEFAULT_BLOCK_NUM 64

DataPool::DataPool()
{	   
	Reset();

	m_dataBlocks.resize(DEFAULT_BLOCK_NUM);
	for (int i = 0; i < DEFAULT_BLOCK_NUM; i++)
	{
		BYTE* dataChunk = new BYTE[m_maxBlockSize];
        memset(dataChunk, 0, m_maxBlockSize);
		m_dataBlocks[i] = dataChunk;
	}
}

DataPool::~DataPool()
{
	for (int i = 0; i < DEFAULT_BLOCK_NUM; i++)
	{
		SAFE_DELETE_ARRAY(m_dataBlocks[i]);
	}
	m_dataBlocks.clear();
}

void DataPool::PutData(const uint8_t* data, uint32_t size)
{
	if (size <= m_curBlockFreeSize)
	{
		memcpy(m_dataBlocks[m_curEmptyBlockIndex] + m_curBlockDataPos, data, size);
		m_curBlockDataPos += size;
		m_curBlockFreeSize -= size;
	}
	else
	{
		memcpy(m_dataBlocks[m_curEmptyBlockIndex] + m_curBlockDataPos, data, m_curBlockFreeSize);

		memcpy(m_dataBlocks[++m_curEmptyBlockIndex], data + m_curBlockFreeSize, size - m_curBlockFreeSize);
		m_curBlockDataPos = size - m_curBlockFreeSize;
		m_curBlockFreeSize = m_maxBlockSize - m_curBlockDataPos;
	}
}

void DataPool::GetAllData(CBufferPtr& dataBufferPtr)
{   
    DWORD dataSize = m_curEmptyBlockIndex*m_maxBlockSize + m_curBlockDataPos;
    dataBufferPtr.Realloc(dataSize);

	DWORD size = 0;
	for (int i = 0; i < m_curEmptyBlockIndex; i++)
	{                
		memcpy(dataBufferPtr.Ptr() + size, m_dataBlocks[i], m_maxBlockSize);
        memset(m_dataBlocks[i], 0, m_maxBlockSize);
        size += m_maxBlockSize;
	}
	memcpy(dataBufferPtr.Ptr() + size, m_dataBlocks[m_curEmptyBlockIndex], m_curBlockDataPos);
    memset(m_dataBlocks[m_curEmptyBlockIndex], 0, m_curBlockDataPos);
    size += m_curBlockDataPos;

	Reset();
}

void DataPool::Reset()
{
	m_curEmptyBlockIndex = 0;
	m_curBlockDataPos = 0;
	m_maxBlockSize = MAX_BLOCK_SIZE;
	m_curBlockFreeSize = m_maxBlockSize;
}
