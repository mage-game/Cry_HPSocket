#include "StdAfx.h"
#include "GameConfiger.h"

void GameConfiger::LoadXmlFile(const char* szFileName)
{	
    m_rootNode = GetISystem()->LoadXmlFromFile(szFileName);
}

string GameConfiger::GetPropertyByName(const char* rootNodeName, const char* nodeName)
{
	XmlNodeRef rootPropertyInfoRef = m_rootNode->findChild(rootNodeName);
	XmlNodeRef nodePropertyRef = rootPropertyInfoRef->findChild(nodeName);

	XmlString property;
	nodePropertyRef->getAttr("value", property);

	return property;
}
