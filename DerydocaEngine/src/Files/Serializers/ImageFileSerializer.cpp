#include "EnginePch.h"
#include "Files\Serializers\ImageFileSerializer.h"
#include "Resources\TextureResource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node ImageFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node materialResource;
		materialResource["ID"] = generateUuid();

		resources.push_back(materialResource);

		return resources;
	}

	Files::FileType ImageFileSerializer::getFileType()
	{
		return Files::FileType::ImageFileType;
	}

	std::shared_ptr<Resources::Resource> ImageFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		// Create a material resource type
		auto r = std::make_shared<Resources::TextureResource>();
		return r;
	}

}
