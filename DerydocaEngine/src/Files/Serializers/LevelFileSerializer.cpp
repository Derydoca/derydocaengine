#include "EnginePch.h"
#include "Files\Serializers\LevelFileSerializer.h"
#include "Resources\LevelResource.h"

namespace DerydocaEngine::Files::Serializers {

	std::vector<std::shared_ptr<Resources::Resource>> LevelFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();
		auto resource = std::make_shared<Resources::LevelResource>();
		resource->generateAndSetId();
		resources.push_back(resource);
		return resources;
	}

	Files::FileType LevelFileSerializer::getFileType()
	{
		return Files::FileType::LevelFileType;
	}

}
