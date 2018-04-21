#include "StdAfx.h"
#include "HPSocketXmlHelper.h"

namespace HPSocket
{
    void XmlHelper::LoadXmlFile(const char* path, const char* name)
    {
        const string szLoadFile = PathUtil::Make(path, name, ".xml");
        m_RootNode = GetISystem()->LoadXmlFromFile(szLoadFile);
    }

    void XmlHelper::GetServerAddress(XmlString& ip, int& port)
    {
        XmlNodeRef serverInfoRef = m_RootNode->findChild("ServerInfo");
        XmlNodeRef addressRef = serverInfoRef->findChild("Adress");
        addressRef->getAttr("ip", ip);
        addressRef->getAttr("port", port);
    }

    void XmlHelper::GetFrameInterval(float& interval)
    {
        XmlNodeRef serverInfoRef = m_RootNode->findChild("ServerInfo");
        XmlNodeRef frameIntervalRef = serverInfoRef->findChild("FrameInterval");
        frameIntervalRef->getAttr("value", interval);
    }
}
