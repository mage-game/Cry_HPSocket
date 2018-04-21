#pragma once

#include "CrySystem\XML\IXml.h"  

namespace HPSocket
{
    class XmlHelper
    {
    public:
        void LoadXmlFile(const char* path, const char* name);

        void GetServerAddress(XmlString& ip, int& port);
        void GetFrameInterval(float& interval);
    private:
        XmlNodeRef m_RootNode;
    };
}
