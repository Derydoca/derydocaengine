#pragma once
#include "Resources\Serializers\MaterialResourceSerializer.h"

namespace DerydocaEngine::Ext
{

	class BezierPatchMeshResourceSerializer : public Resources::Serializers::MaterialResourceSerializer
	{
	public:
		BezierPatchMeshResourceSerializer() {}
		~BezierPatchMeshResourceSerializer() {}

		virtual void* deserialize(Resources::Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resources::Resource* const& resource);
		virtual Resources::ResourceType getResourceType();
	};

}
