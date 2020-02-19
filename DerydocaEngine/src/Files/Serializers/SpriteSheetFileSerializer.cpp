#include "EnginePch.h"
#include "Files\Serializers\SpriteSheetFileSerializer.h"
#include "Resources\SpriteSheetResource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node SpriteSheetFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node spriteSheetResource;
		spriteSheetResource["ID"] = generateUuid();

		resources.push_back(spriteSheetResource);

		return resources;
	}

	std::vector<std::shared_ptr<Resources::Resource>> SpriteSheetFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();
		auto resource = std::make_shared<Resources::SpriteSheetResource>();
		resource->generateAndSetId();
		resources.push_back(resource);
		return resources;
	}

	Files::FileType SpriteSheetFileSerializer::getFileType()
	{
		return Files::FileType::SpriteSheetFileType;
	}

	std::shared_ptr<Resources::Resource> SpriteSheetFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		auto r = std::make_shared<Resources::SpriteSheetResource>();
		return r;
	}

}
