#include "Resource.h"
#include "ResourceSerializerLibrary.h"

Resource::Resource()
{

}

Resource::Resource(uuid id, std::string sourceFilePath, std::string metaFilePath) :
	m_id(id),
	m_sourceFilePath(sourceFilePath),
	m_metaFilePath(metaFilePath)
{
	m_type = pathToResourceType(sourceFilePath);
	m_resourceObject = 0;
}

Resource::Resource(uuid id, std::string sourceFilePath, std::string metaFilePath, ResourceType type) :
	m_id(id),
	m_sourceFilePath(sourceFilePath),
	m_metaFilePath(metaFilePath)
{
	m_type = type;
	m_resourceObject = 0;
}

void* Resource::getResourceObject() {
	if (m_resourceObject == 0)
	{
		// Find the loader that should be used
		ResourceTypeSerializer* loader = ResourceSerializerLibrary::getInstance().getTypeSerializer(getType());

		// If the loader could not be found, return null
		if (loader == nullptr)
		{
			return nullptr;
		}

		// Load the object from the related object loader and return it
		return loader->deserialize(this);
		//m_resourceObject = ResourceSerializer::getInstance().loadResource(this);
	}

	return m_resourceObject;
}