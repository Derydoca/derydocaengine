#include "Resource.h"

Resource::Resource(int id, std::string sourceFilePath, std::string metaFilePath) :
	m_id(id),
	m_sourceFilePath(sourceFilePath),
	m_metaFilePath(metaFilePath)
{
	m_type = pathToResourceType(sourceFilePath);
	m_resourceObject = 0;
}

void* Resource::getResourceObject() {
	if (m_resourceObject == 0)
	{
		m_resourceObject = ResourceSerializer::getInstance().loadResource(this);
	}

	return m_resourceObject;
}