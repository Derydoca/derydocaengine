#include "Files\Serializers\BezierPatchMeshFileSerializer.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Files::Serializers {

	YAML::Node BezierPatchMeshFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node bezierPatchMeshResource;
		bezierPatchMeshResource["ID"] = generateUuid();

		resources.push_back(bezierPatchMeshResource);

		return resources;
	}

	Files::FileType BezierPatchMeshFileSerializer::getFileType()
	{
		return Files::FileType::BezierPatchMeshType;
	}

	std::shared_ptr<Resources::Resource> BezierPatchMeshFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		auto r = std::make_shared<Resources::Resource>();
		r->setType(Resources::BezierPatchMeshResourceType);
		return r;
	}

}
