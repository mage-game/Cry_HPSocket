#pragma once   
#pragma warning (disable:4200)

#include <stdint.h>

#pragma pack(push,2)
struct PkgHeader
{
    uint64_t connID;
    uint32_t bodyType;
    uint32_t bodyLen;
    uint8_t body[0];
    PkgHeader()
    {
        connID = 0;
        bodyType = 0;
        bodyLen = 0;
    }
};
#pragma pack(pop)     
