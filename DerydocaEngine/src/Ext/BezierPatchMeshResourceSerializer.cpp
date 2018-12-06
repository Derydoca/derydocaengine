#include "BezierPatchMeshResourceSerializer.h"
#include "BezierPatchMeshFileLoader.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Ext
{

	std::shared_ptr<void> BezierPatchMeshResourceSerializer::deserializePointer(std::shared_ptr<Resources::Resource> resource)
	{
		BezierPatchMeshFileLoader loader;
		auto mesh = loader.Load(resource->getSourceFilePath().c_str());
		return mesh;
	}

	Resources::ResourceType BezierPatchMeshResourceSerializer::getResourceType()
	{
		return Resources::ResourceType::BezierPatchMeshResourceType;
	}

}
