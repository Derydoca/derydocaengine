#include "BezierPatchMeshResourceSerializer.h"
#include "BezierPatchMeshFileLoader.h"
#include "Resource.h"

namespace DerydocaEngine::Ext
{

	void * BezierPatchMeshResourceSerializer::deserialize(Resources::Resource * const& resource)
	{
		BezierPatchMeshFileLoader loader;
		BezierPatchMesh* mesh = loader.Load(resource->getSourceFilePath().c_str());
		return mesh;
	}

	Resources::ResourceType BezierPatchMeshResourceSerializer::getResourceType()
	{
		return Resources::ResourceType::BezierPatchMeshResourceType;
	}

}
