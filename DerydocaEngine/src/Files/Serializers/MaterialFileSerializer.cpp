#include "EnginePch.h"
#include "Files\Serializers\MaterialFileSerializer.h"
#include "Rendering\Material.h"
#include "Rendering\Shader.h"
#include "Resources\CubemapResource.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Files::Serializers {

	Files::FileType MaterialSerializer::getFileType()
	{
		return Files::FileType::MaterialFileType;
	}

	std::vector<std::shared_ptr<Resources::Resource>> MaterialSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();

		auto resource = std::make_shared<Resources::MaterialResource>();
		resource->generateAndSetId();
		resources.push_back(resource);

		return resources;
	}

}
