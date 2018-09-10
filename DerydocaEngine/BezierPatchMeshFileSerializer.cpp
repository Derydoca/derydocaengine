#include "BezierPatchMeshFileSerializer.h"
#include "Resource.h"

BezierPatchMeshFileSerializer::BezierPatchMeshFileSerializer()
{
}

BezierPatchMeshFileSerializer::~BezierPatchMeshFileSerializer()
{
}

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

Resource * BezierPatchMeshFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
{
	Resource* r = new Resource();
	r->setType(BezierPatchMeshResourceType);
	return r;
}
