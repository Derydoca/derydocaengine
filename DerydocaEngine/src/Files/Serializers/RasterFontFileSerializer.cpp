#include "EnginePch.h"
#include "Files\Serializers\RasterFontFileSerializer.h"
#include "Resources\RasterFontResource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node RasterFontFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	std::vector<std::shared_ptr<Resources::Resource>> RasterFontFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();
		auto resource = std::make_shared<Resources::RasterFontResource>();
		resource->generateAndSetId();
		resources.push_back(resource);
		return resources;
	}

	Files::FileType RasterFontFileSerializer::getFileType()
	{
		return Files::FileType::RasterFontType;
	}

}
