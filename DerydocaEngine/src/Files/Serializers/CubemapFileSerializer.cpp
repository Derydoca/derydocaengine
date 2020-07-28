#include "EnginePch.h"
#include "Files\Serializers\CubemapFileSerializer.h"
#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "Resources\CubemapResource.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Files::Serializers {

	Files::FileType CubemapSerializer::getFileType()
	{
		return Files::FileType::CubemapFileType;
	}

	std::vector<std::shared_ptr<Resources::Resource>> CubemapSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();

		auto resource = std::make_shared<Resources::CubemapResource>();
		resource->generateAndSetId();
		resources.push_back(resource);

		return resources;
	}

}
