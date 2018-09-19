#include "BezierPatchMeshFileSerializer.h"
#include "Resource.h"

namespace DerydocaEngine::FileSerializers {

	YAML::Node BezierPatchMeshFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node bezierPatchMeshResource;
		bezierPatchMeshResource["ID"] = generateUuid();

		resources.push_back(bezierPatchMeshResource);

		return resources;
	}

	FileType BezierPatchMeshFileSerializer::getFileType()
	{
		return FileType::BezierPatchMeshType;
	}

	Resources::Resource * BezierPatchMeshFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::Resource* r = new Resources::Resource();
		r->setType(Resources::BezierPatchMeshResourceType);
		return r;
	}

}
