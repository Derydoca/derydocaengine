#include "BezierPatchMeshResourceSerializer.h"
#include "BezierPatchMeshFileLoader.h"
#include "Resources\Resource.h"

namespace DerydocaEngine::Ext
{

	void * BezierPatchMeshResourceSerializer::deserialize(Resources::Resource * const& resource)
	{
		return nullptr;
	}

	std::shared_ptr<void> BezierPatchMeshResourceSerializer::deserializePointer(Resources::Resource * const & resource)
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
