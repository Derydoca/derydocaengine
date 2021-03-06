#include "EnginePch.h"
#include "Files\Serializers\SpriteSheetFileSerializer.h"
#include "Resources\SpriteSheetResource.h"

namespace DerydocaEngine::Files::Serializers {

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

}
