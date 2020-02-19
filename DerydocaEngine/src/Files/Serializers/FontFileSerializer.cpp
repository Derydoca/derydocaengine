#include "EnginePch.h"
#include "Files\Serializers\FontFileSerializer.h"
#include "Resources\FontResource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node FontFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node levelResource;
		levelResource["ID"] = generateUuid();

		resources.push_back(levelResource);

		return resources;
	}

	std::vector<std::shared_ptr<Resources::Resource>> FontFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();
		auto resource = std::make_shared<Resources::FontResource>();
		resource->generateAndSetId();
		resources.push_back(resource);
		return resources;
	}

	Files::FileType FontFileSerializer::getFileType()
	{
		return Files::FileType::FontType;
	}

	std::shared_ptr<Resources::Resource> FontFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		auto r = std::make_shared<Resources::FontResource>();
		return r;
	}

}
