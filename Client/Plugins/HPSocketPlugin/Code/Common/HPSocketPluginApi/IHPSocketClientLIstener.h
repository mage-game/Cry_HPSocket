#pragma once
#include "HPSocketSerializer.h"

namespace HPSocket
{
    class IClientListener
    {
    public:
		virtual ~IClientListener(){}
        virtual void Serialize(Serializer& serializer){}
        virtual void Unserialize(Serializer& unserializer){}
    };
}