#pragma once   
#pragma warning (disable:4200)

#include <stdint.h>

#pragma pack(push,2)
struct PkgHeader
{
	uint64_t connID = 0;
	uint32_t bodyType = 0;
	uint32_t bodyLen = 0;
	uint8_t body[0];
};
#pragma pack(pop)

 
