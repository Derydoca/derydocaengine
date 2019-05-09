#include "EnginePch.h"
#include "Resources\Resource.h"
#include "Files\Serializers\FileSerializerLibrary.h"
#include "Resources\Serializers\ResourceSerializerLibrary.h"

namespace DerydocaEngine::Resources
{

	Resource::Resource() :
		Resource("", ResourceType::UnknownResourceType, "", "")
	{

	}

	Resource::Resource(const boost::uuids::uuid& id, const std::string& sourceFilePath, const std::string& metaFilePath, const ResourceType type) :
		Resource(id, "", type, sourceFilePath, metaFilePath)
	{
		auto filePath = boost::filesystem::path(sourceFilePath);
		if (filePath.has_stem())
		{
			m_name = filePath.stem().string();
		}
	}

	Resource::Resource(const std::string & name, const ResourceType type, const std::string & sourceFilePath, const std::string & metaFilePath) :
		Object(),
		m_name(name),
		m_type(type),
		m_sourceFilePath(sourceFilePath),
		m_metaFilePath(metaFilePath),
		m_resourceObjectPointer(nullptr)
	{
	}

	Resource::Resource(const boost::uuids::uuid& id, const std::string & name, const ResourceType type, const std::string & sourceFilePath, const std::string & metaFilePath) :
		Object(id),
		m_name(name),
		m_type(type),
		m_sourceFilePath(sourceFilePath),
		m_metaFilePath(metaFilePath),
		m_resourceObjectPointer(nullptr)
	{
	}

	void Resource::setFilePaths(const std::string& sourceFilePath, const std::string& metaFilePath)
	{
		m_sourceFilePath = sourceFilePath;
		m_metaFilePath = metaFilePath;

		auto filePath = boost::filesystem::path(sourceFilePath);
		if (filePath.has_stem())
		{
			m_name = filePath.stem().string();
		}
	}

	std::shared_ptr<void> Resource::getResourceObjectPointer()
	{
		if (m_resourceObjectPointer == 0)
		{
			// Find the loader that should be used
			auto loader = Serializers::ResourceSerializerLibrary::getInstance().getSerializer(getType());

			// If the loader could not be found, return null
			if (loader == nullptr)
			{
				return nullptr;
			}

			// Load the object from the related object loader and return it
			m_resourceObjectPointer = loader->deserializePointer(shared_from_this());
		}

		return m_resourceObjectPointer;
	}

}
