#include "stdafx.h"
#include "DataPool.h"

#define MAX_BLOCK_SIZE (64*1024)
#define DEFAULT_BLOCK_NUM 64

DataPool::DataPool()
{	   
	Reset();

	m_DataBlocks.resize(DEFAULT_BLOCK_NUM);
	for (int i = 0; i < DEFAULT_BLOCK_NUM; i++)
	{
		BYTE* dataChunk = new BYTE[m_MaxBlockSize];
        memset(dataChunk, 0, m_MaxBlockSize);
		m_DataBlocks[i] = dataChunk;
	}
}

DataPool::~DataPool()
{
	for (int i = 0; i < DEFAULT_BLOCK_NUM; i++)
	{
		SAFE_DELETE_ARRAY(m_DataBlocks[i]);
	}
	m_DataBlocks.clear();
}

void DataPool::PutData(const uint8_t* data, uint32_t size)
{
	if (size <= m_CurBlockFreeSize)
	{
		memcpy(m_DataBlocks[m_CurEmptyBlockIndex] + m_CurBlockDataPos, data, size);
		m_CurBlockDataPos += size;
		m_CurBlockFreeSize -= size;
	}
	else
	{
		memcpy(m_DataBlocks[m_CurEmptyBlockIndex] + m_CurBlockDataPos, data, m_CurBlockFreeSize);

		memcpy(m_DataBlocks[++m_CurEmptyBlockIndex], data + m_CurBlockFreeSize, size - m_CurBlockFreeSize);
		m_CurBlockDataPos = size - m_CurBlockFreeSize;
		m_CurBlockFreeSize = m_MaxBlockSize - m_CurBlockDataPos;
	}
}

void DataPool::GetAllData(CBufferPtr& dataBufferPtr)
{   
    DWORD dataSize = m_CurEmptyBlockIndex*m_MaxBlockSize + m_CurBlockDataPos;
    dataBufferPtr.Realloc(dataSize);

	DWORD size = 0;
	for (int i = 0; i < m_CurEmptyBlockIndex; i++)
	{                
		memcpy(dataBufferPtr.Ptr() + size, m_DataBlocks[i], m_MaxBlockSize);
        memset(m_DataBlocks[i], 0, m_MaxBlockSize);
        size += m_MaxBlockSize;
	}
	memcpy(dataBufferPtr.Ptr() + size, m_DataBlocks[m_CurEmptyBlockIndex], m_CurBlockDataPos);
    memset(m_DataBlocks[m_CurEmptyBlockIndex], 0, m_CurBlockDataPos);
    size += m_CurBlockDataPos;

	Reset();
}

void DataPool::Reset()
{
	m_CurEmptyBlockIndex = 0;
	m_CurBlockDataPos = 0;
	m_MaxBlockSize = MAX_BLOCK_SIZE;
	m_CurBlockFreeSize = m_MaxBlockSize;
}
