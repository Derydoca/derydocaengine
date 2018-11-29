#include "Resources\Resource.h"
#include "Files\Serializers\FileSerializerLibrary.h"
#include "Resources\Serializers\ResourceSerializerLibrary.h"

namespace DerydocaEngine::Resources
{

	Resource::Resource()
	{

	}

	Resource::Resource(boost::uuids::uuid const& id, std::string const& sourceFilePath, std::string const& metaFilePath, ResourceType const& type) :
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
			Serializers::ResourceSerializer* loader = Serializers::ResourceSerializerLibrary::getInstance().getSerializer(getType());

			// If the loader could not be found, return null
			if (loader == nullptr)
			{
				return nullptr;
			}

			// Load the object from the related object loader and return it
			m_resourceObject = loader->deserialize(this);
		}

		return m_resourceObject;
	}

	std::shared_ptr<void> Resource::getResourceObjectPointer()
	{
		if (m_resourceObjectPointer == 0)
		{
			// Find the loader that should be used
			Serializers::ResourceSerializer* loader = Serializers::ResourceSerializerLibrary::getInstance().getSerializer(getType());

			// If the loader could not be found, return null
			if (loader == nullptr)
			{
				return nullptr;
			}

			// Load the object from the related object loader and return it
			m_resourceObjectPointer = loader->deserializePointer(this);
		}

		return m_resourceObjectPointer;
	}

}