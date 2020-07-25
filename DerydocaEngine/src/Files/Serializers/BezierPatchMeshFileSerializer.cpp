#include "EnginePch.h"
#include "Files\Serializers\BezierPatchMeshFileSerializer.h"
#include "Ext\BezierPatchMeshResource.h"

namespace DerydocaEngine::Files::Serializers {

	std::vector<std::shared_ptr<Resources::Resource>> BezierPatchMeshFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();
		auto resource = std::make_shared<Resources::BezierPatchMeshResource>();
		resource->generateAndSetId();
		resources.push_back(resource);
		return resources;
	}

	Files::FileType BezierPatchMeshFileSerializer::getFileType()
	{
		return Files::FileType::BezierPatchMeshType;
	}

}
