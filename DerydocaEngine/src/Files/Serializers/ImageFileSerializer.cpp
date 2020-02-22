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

	std::vector<std::shared_ptr<Resources::Resource>> ImageFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();
		auto resource = std::make_shared<Resources::TextureResource>();
		resource->generateAndSetId();
		resources.push_back(resource);
		return resources;
	}

	Files::FileType ImageFileSerializer::getFileType()
	{
		return Files::FileType::ImageFileType;
	}

}
