#include "BezierPatchMeshResourceSerializer.h"
#include "BezierPatchMeshFileLoader.h"
#include "Resource.h"

namespace DerydocaEngine::Ext
{

	void * BezierPatchMeshResourceSerializer::deserialize(Resource * const& resource)
	{
		BezierPatchMeshFileLoader loader;
		BezierPatchMesh* mesh = loader.Load(resource->getSourceFilePath().c_str());
		return mesh;
	}

	ResourceType BezierPatchMeshResourceSerializer::getResourceType()
	{
		return ResourceType::BezierPatchMeshResourceType;
	}

}
