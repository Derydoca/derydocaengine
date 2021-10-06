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

	Resource::Resource(ResourceType type) :
		Resource("", type, "", "")
	{
	}

	Resource::Resource(const uuids::uuid& id, const std::filesystem::path& sourceFilePath, const std::filesystem::path& metaFilePath, const ResourceType type) :
		Resource(id, "", type, sourceFilePath, metaFilePath)
	{
		auto filePath = std::filesystem::path(sourceFilePath);
		if (filePath.has_stem())
		{
			m_name = filePath.stem().string();
		}
	}

	Resource::Resource(const std::string & name, const ResourceType type, const std::filesystem::path& sourceFilePath, const std::filesystem::path& metaFilePath) :
		Object(),
		m_name(name),
		m_type(type),
		m_sourceFilePath(sourceFilePath),
		m_metaFilePath(metaFilePath),
		m_resourceObjectPointer(nullptr)
	{
	}

	Resource::Resource(const uuids::uuid& id, const std::string & name, const ResourceType type, const std::filesystem::path& sourceFilePath, const std::filesystem::path& metaFilePath) :
		Object(id),
		m_name(name),
		m_type(type),
		m_sourceFilePath(sourceFilePath),
		m_metaFilePath(metaFilePath),
		m_resourceObjectPointer(nullptr)
	{
	}

	void Resource::generateAndSetId()
	{
		static uuids::uuid_time_generator gen;
		m_ID = gen();
	}

	void Resource::setFilePaths(const std::filesystem::path& sourceFilePath, const std::filesystem::path& metaFilePath)
	{
		m_sourceFilePath = sourceFilePath;
		m_metaFilePath = metaFilePath;

		auto filePath = std::filesystem::path(sourceFilePath);
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
