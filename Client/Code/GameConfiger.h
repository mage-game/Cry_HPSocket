#pragma once

#include "CrySystem\XML\IXml.h"

class GameConfiger
{
public:     
    void LoadXmlFile(const char* szFileName);

    string GetPropertyByName(const char* rootNodeName,const char* nodeName);
private:
    XmlNodeRef m_rootNode;
};
