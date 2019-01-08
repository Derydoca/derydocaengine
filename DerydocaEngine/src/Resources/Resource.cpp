#include "EnginePch.h"
#include "Resources\Resource.h"
#include "Files\Serializers\FileSerializerLibrary.h"
#include "Resources\Serializers\ResourceSerializerLibrary.h"

namespace DerydocaEngine::Resources
{

	Resource::Resource() :
		Object(),
		m_name(),
		m_type(ResourceType::UnknownResourceType),
		m_sourceFilePath(),
		m_metaFilePath(),
		m_resourceObject(nullptr),
		m_resourceObjectPointer()
	{

	}

	Resource::Resource(boost::uuids::uuid const& id, std::string const& sourceFilePath, std::string const& metaFilePath, ResourceType const& type) :
		Object(id),
		m_sourceFilePath(sourceFilePath),
		m_metaFilePath(metaFilePath)
	{
		auto filePath = boost::filesystem::path(sourceFilePath);
		if (filePath.has_stem())
		{
			m_name = filePath.stem().string();
		}

		m_type = type;
		m_resourceObject = 0;
	}

	void Resource::setFilePaths(std::string const & sourceFilePath, std::string const & metaFilePath)
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
