#include "Resource.h"
#include "FileSerializerLibrary.h"
#include "ResourceSerializerLibrary.h"

Resource::Resource()
{

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
		ResourceSerializer* loader = ResourceSerializerLibrary::getInstance().getSerializer(getType());

		// If the loader could not be found, return null
		if (loader == nullptr)
		{
			return nullptr;
		}

		// Load the object from the related object loader and return it
		return loader->deserialize(this);
	}

	return m_resourceObject;
}