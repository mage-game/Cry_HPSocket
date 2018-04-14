#pragma once

#include <vector>
#include <stdint.h>
#include <bufferptr.h>

class DataPool
{
public:
	DataPool();	

	~DataPool();

    void PutData(const uint8_t* data, uint32_t size);

    void GetAllData(CBufferPtr& dataBufferPtr);
private:
	void Reset();
private:
	std::vector<uint8_t*> m_DataBlocks;
	uint32_t m_MaxBlockSize;
	int m_CurEmptyBlockIndex;
    uint32_t m_CurBlockDataPos;
    uint32_t m_CurBlockFreeSize;
};
