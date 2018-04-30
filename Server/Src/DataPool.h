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
	std::vector<uint8_t*> m_dataBlocks;
	uint32_t m_maxBlockSize;
	int m_curEmptyBlockIndex;
    uint32_t m_curBlockDataPos;
    uint32_t m_curBlockFreeSize;
};
